from app import *

from flask_login import LoginManager, current_user, login_user, logout_user

# Define user login functionality.
loginManager = LoginManager()
loginManager.init_app(app)
loginManager.login_view = "login"

@loginManager.user_loader
def load_user(id: int):
	"""
	Load a user using their id.
	"""

	return User.query.get(id)

@app.route("/login", methods=["POST"])
def login():
	"""
	Authenticate the user. User must supply a valid card ID, which is an 8-digit
	hexadecimal string.
	"""

	user = User.login(request.form["cardID"])

	# Failed login condition.
	if user == False:
		return render_template("index.html", failed=True)

	login_user(user)
	return redirect(url_for("application"))

