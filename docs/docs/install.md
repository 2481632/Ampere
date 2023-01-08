# Install 

## Webservices

### Install requirements

Install all dependencys.

```
pip -r requirements.txt
```

### Initialize project

```
python manage.py makemigrations api
python manage.py migrate
```
### Create superuser

If not already done, execute the following command to create a superuser. You will need one to add devices to the device list. 

```
python manage.py createsuperuser
```

### Run dev server

Start a dev server on port 8000.

```
python manage.py runserver
```

### Add devices to device list 

Visit ```http://127.0.0.1:8000/admin/``` and log in with you created user. Navigate to ```devices``` to add devices.


