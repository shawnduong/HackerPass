import sys

from flask import *
from flask_sqlalchemy import *
from flask_login import LoginManager, current_user, login_user, logout_user

from keys import FLASK_SECRET_KEY

# Instantiate the application and define settings.
app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///database.sqlite"
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
app.secret_key = FLASK_SECRET_KEY

# Load the database.
db = SQLAlchemy(app)
from models import *
with app.app_context():
	db.create_all()

# Authentication.
from authentication import *

# Website routes.
from routes import *

# API endpoints, used by HackerPass units.
from api import *

# AJAX endpoints, used by web app users.
from ajax import *
