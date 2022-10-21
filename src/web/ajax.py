from app import *
from sqlalchemy import *

@app.route("/ajax/user/update/name", methods=["POST"])
@login_required
def app_user_update_name():
	"""
	Update a user's name.
	"""

	try:
		User.query.get(current_user.id).name = request.form["name"]
		db.session.commit()
		return {"Status": "Name updated."}, 200
	except Exception as e:
		print(e)
		return {"Status": "Failure."}, 500

@app.route("/ajax/user/update/email", methods=["POST"])
@login_required
def app_user_update_email():
	"""
	Update a user's email.
	"""

	try:
		User.query.get(current_user.id).email = request.form["email"]
		db.session.commit()
		return {"Status": "Email updated."}, 200
	except:
		return {"Status": "Failure."}, 500

@app.route("/ajax/user/info", methods=["GET"])
@login_required
def app_user_info():
	"""
	Return a JSON object of a user's points and the events they've attended as
	well as upcoming events.
	"""

	try:
		User.query.get(current_user.id).update_points()
		return {
			"Points": User.query.get(current_user.id).points,
			"Status": "Success."
		}, 200
	except:
		return {"Status": "Failure."}, 500

