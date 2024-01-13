local config = {
    interval_seconds = 15,
    expected_minutes_max = 17,
    port = "/dev/serial/by-path/pci-0000:00:14.0-usbv2-0:2.1:1.0-port0",
    stty =
    "115200 -parenb -parodd -cmspar cs8 -hupcl -cstopb cread clocal -crtscts -ignbrk brkint ignpar -parmrk -inpck -istrip -inlcr -igncr -icrnl ixon -ixoff -iuclc -ixany -imaxbel -iutf8 -opost -olcuc -ocrnl -onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0 -isig -icanon iexten -echo echoe -echonl -noflsh -xcase -tostop -echoprt echoctl echoke -flusho -extproc"

}

for k, v in ipairs(arg) do
    if (v == "--port") then
        config.port = arg[k + 1]
    end

    if (v == "-stty") then
        os.execute(table.concat({
            "stty -F",
            config.port,
            config.stty
        }, " "))
    end
end

return config
