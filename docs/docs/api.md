# API

A simple rest-API is used to provide a simple way to save, update and get energy usage data. 

### Make api calls

GET and POST are allowed. GET will return a list of all entries made by device. 

```
http://127.0.0.1:8000/api/device/<device_id>/usage/
```

