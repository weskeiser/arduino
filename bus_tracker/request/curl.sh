curl https://api.entur.io/journey-planner/v3/graphql \
    -H 'Content-Type: application/json' \
    -H 'ET-Client-Name: wes-home-board' \
    -X POST \
    -T ./req.json \
    | jq '.data.trip.tripPatterns.[] | .legs.[]'
