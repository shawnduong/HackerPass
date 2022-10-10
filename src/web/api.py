from app import *
from sqlalchemy import *

@app.route("/api/users", methods=["GET"])
def get_users():
	"""
	Give the user a JSON list of users' IDs.
	"""

	return {"USERS": [u.cardID for u in User.query.all()]}, 200

@app.route("/api/users/create", methods=["POST"])
def create_user():
	"""
	Create a new user with some cardID and 0 points.
	"""

	cardID = request.json["cardID"]

	try:
		user = User(cardID, 0)
		db.session.add(user)
		db.session.commit()
	except:
		return {"STATUS": "FAILURE"}, 500

	return {"STATUS": "SUCCESS"}, 200

