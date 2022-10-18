from app import *

from flask_login import current_user, login_required

@app.route("/", methods=["GET"])
def index():
	"""
	Display the index page to the user.
	"""

	return render_template("index.html")

@app.route("/app", methods=["GET"])
@login_required
def application():
	"""
	Display the logged-in account page to the user.
	"""

	return render_template("app.html")

