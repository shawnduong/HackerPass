from app import *
from sqlalchemy import *

@app.route("/api/users", methods=["GET"])
def get_users():
	"""
	Give the user a JSON list of users' IDs.
	"""

	return {"USERS": [u.cardID for u in User.query.all()]}

