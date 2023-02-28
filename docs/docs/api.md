# API

A simple rest-API is used to provide a simple way to save, update and get energy usage data. 

## Make api calls

### POST data to database 

GET and POST are allowed. GET will return a list of all entries made by device. 

```
http://127.0.0.1:8000/api/device/<device_id>/usage/
```
#### Example 

```
GET /api/device/52c13cb7-cb25-4818-a9a3-db18e4913ab0/usage/

HTTP 200 OK
Allow: GET, POST, HEAD, OPTIONS
Content-Type: application/json
Vary: Accept

[
    {
        "id": 5,
        "device": {
            "id": "52c13cb7-cb25-4818-a9a3-db18e4913ab0",
            "created": "2023-01-31T13:19:38.003111Z"
        },
        "loadDevice": {
            "id": 1,
            "label": "Spülmaschiene"
        },
        "created": "2023-01-31T21:54:11.504885Z",
        "usage": 10
    },
    {
        "id": 4,
        "device": {
            "id": "52c13cb7-cb25-4818-a9a3-db18e4913ab0",
            "created": "2023-01-31T13:19:38.003111Z"
        },
        "loadDevice": {
            "label": ""
        },
        "created": "2023-01-31T14:18:26.159106Z",
        "usage": 66
    }
}
```

### Get computed values of device

Only ```GET``` is allowed. 

```/api/device/<device_id>/computed/```

This will return a json object containing the current, avg day, avg month and avg year values. Some values may be __null__ if no coresponding entries are found.

#### Example

```
GET /api/device/52c13cb7-cb25-4818-a9a3-db18e4913ab0/computed/

HTTP 200 OK
Allow: GET, HEAD, OPTIONS
Content-Type: application/json
Vary: Accept

{
    "now": null,
    "day": null,
    "month": 70,
    "year": 70
}
```


