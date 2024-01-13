#!/usr/bin/env lua

local config = require("config")

local function fetch_data()
    local res = assert(io.popen("./curl.sh"), "Error: Data request failed")

    local rides = {}
    repeat
        local line = res:read()
        if not line then break end

        if #line ~= 1 then -- Possible values are "}" and "{"
            local kv_pair = line:gmatch('%b""')
            local k, v = kv_pair():sub(2, -2), kv_pair():sub(2, -2);

            if k == "expectedStartTime" then
                local d, h, m = v:sub(9, 16):match("(%d-)T(%d-):(%d+)");

                table.insert(rides, {
                    time_dhms_raw = tonumber(table.concat({ d, h, m })),
                    time_dhms_tbl = { day = tonumber(d), hour = tonumber(h), min = tonumber(m) }
                })
            end
        end
    until not line

    table.sort(rides, function(a, b)
        return a.time_dhms_raw > b.time_dhms_raw
    end)

    return rides
end

local function seconds_sleep(seconds)
    print(table.concat({ "Going to sleep for", seconds, "seconds..\n" }, " "))
    local sleep = os.execute("sleep " .. seconds)
    return sleep
end

local function dhms_min_left(dhms)
    local date = os.date("!*t")
    local os_ms = os.time(table.unpack(date))

    local data_ms = os.time({
        year = date.year,
        month = date.month,
        day = dhms.day,
        hour = dhms.hour,
        min = dhms.min,
    })

    return math.floor(os.difftime(data_ms, os_ms) / 60)
end


local function rides_process(rides)
    local min_left_tbl = {}

    for _, ride in ipairs(rides) do
        local min_left = dhms_min_left(ride.time_dhms_tbl)

        if (min_left <= config.expected_minutes_max) and (min_left > 0) then
            print("Minutes left:", min_left)
            local min_left_formatted = string.format("\\x%2x", min_left):gsub(" ", 0)
            table.insert(min_left_tbl, min_left_formatted)
        end
    end

    local data = table.concat(table.unpack({ min_left_tbl })) or 0x00
    return string.format("%q", data)
end


local function data_transmit(data)
    assert(os.execute(table.concat({ "printf ", data, " > ", config.port })),
        "Error attempting to communicate with" .. config.port)
end

repeat
    local rides = fetch_data();

    if #rides then
        data_transmit(rides_process(rides))
    end

    local sleep_successful = seconds_sleep(config.interval_seconds);
until not sleep_successful
