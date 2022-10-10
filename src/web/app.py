import sys

from flask import *
from flask_sqlalchemy import *

from keys import FLASK_SECRET_KEY

# Instantiate the application and define settings.
app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///database.sqlite"
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
app.secret_key = FLASK_SECRET_KEY

# Load the database.
db = SQLAlchemy(app)
from models import *
db.create_all()

# API endpoints.
from api import *
