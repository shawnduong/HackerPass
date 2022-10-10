from app import *
from sqlalchemy import *

@app.route("/api/user", methods=["GET"])
def get_users():
	"""
	Return a JSON list of users' IDs.
	"""

	return {"USERS": [u.cardID for u in User.query.all()]}, 200

@app.route("/api/user/create", methods=["POST"])
def create_user():
	"""
	Create a new user with some cardID.
	"""

	try:
		user = User(request.json["cardID"])
		db.session.add(user)
		db.session.commit()
	except:
		return {"STATUS": "FAILURE"}, 500

	return {"STATUS": "SUCCESS"}, 200

@app.route("/api/event", methods=["GET"])
def get_events():
	"""
	Return a JSON list of all events.
	"""

	events = []

	for e in Event.query.all():
		events.append({k:v for k,v in e.__dict__.items() if k != "_sa_instance_state"})

	return {"EVENTS": events}, 200

@app.route("/api/event/create", methods=["POST"])
def create_event():
	"""
	Create a new event with some number of associated points.
	"""

	try:
		event = Event(
			request.json["points"],
			request.json["title"],
			request.json["about"],
			request.json["room"],
			request.json["author"],
		)
		db.session.add(event)
		db.session.commit()
	except:
		return {"STATUS": "FAILURE"}, 500

	return {"STATUS": "SUCCESS"}, 200

@app.route("/api/event/update", methods=["POST"])
def update_event():
	"""
	Update an event.
	"""

	try:
		Event.query.filter_by(id=request.json["id"]).update(request.json)
		db.session.commit()
	except Exception as e:
		print(e)
		return {"STATUS": "FAILURE"}, 500

	return {"STATUS": "SUCCESS"}, 200

@app.route("/api/event/delete", methods=["POST"])
def delete_event():
	"""
	Delete an event based on its ID.
	"""

	try:
		Event.query.filter_by(id=request.json["id"]).delete()
		db.session.commit()
	except:
		return {"STATUS": "FAILURE"}, 500

	return {"STATUS": "SUCCESS"}, 200

