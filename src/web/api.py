from app import *
from sqlalchemy import *

from keys import HP_KEYS

@app.route("/api/hp/user", methods=["GET"])
def get_users():
	"""
	Return a list of all user card IDs.
	"""

	try:
		assert int(request.args.get("key")) in HP_KEYS
	except:
		return {"Status": "Authentication invalid."}, 403

	try:
		return {
			"Status": "Success.",
			"CardIDs": [u.cardID for u in User.query.all()]
		}, 200
	except:
		return {"Status": "Failure."}, 500

@app.route("/api/hp/user/<cardID>", methods=["GET"])
def get_user(cardID):
	"""
	Return everything about a specific user.
	"""

	try:
		assert int(request.args.get("key")) in HP_KEYS
	except:
		return {"Status": "Authentication invalid."}, 403

	try:
		user = User.query.filter_by(cardID=cardID).first()
		assert user is not None
		user.update_points()
		return {
			"Status": "Success.",
			cardID: {k:v for k,v in user.__dict__.items() if k != "_sa_instance_state"}
		}, 200
	except AssertionError:
		return {"Status": "User does not exist."}, 404
	except:
		return {"Status": "Failure."}, 500

@app.route("/api/hp/user/create", methods=["POST"])
def create_user():
	"""
	Create a new user with some card ID. Name and email fields are filled out
	by the user later on after registration.
	"""

	try:
		assert int(request.args.get("key")) in HP_KEYS
	except:
		return {"Status": "Authentication invalid."}, 403

	try:
		assert User.query.filter_by(cardID=request.json["cardID"]).first() is None
		user = User(request.json["cardID"])
		db.session.add(user)
		db.session.commit()
		return {"Status": "User created."}, 200
	except AssertionError:
		return {"Status": "User already exists."}, 400
	except:
		return {"Status": "Failure."}, 500

@app.route("/api/hp/event", methods=["GET"])
def get_events():
	"""
	Return a JSON list of all events.
	"""

	try:
		assert int(request.args.get("key")) in HP_KEYS
	except:
		return {"Status": "Authentication invalid."}, 403

	events = []

	for e in Event.query.all():
		events.append({k:v for k,v in e.__dict__.items() if k != "_sa_instance_state"})

	return {
		"Status": "Success.",
		"Events": events
	}, 200

@app.route("/api/hp/event/create", methods=["POST"])
def create_event():
	"""
	Create a new event with some number of associated points.
	"""

	try:
		assert int(request.args.get("key")) in HP_KEYS
	except:
		return {"Status": "Authentication invalid."}, 403

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
		return {"Status": "Event created."}, 200
	except:
		return {"Status": "Failure."}, 500

@app.route("/api/hp/event/update", methods=["POST"])
def update_event():
	"""
	Update an event.
	"""

	try:
		assert int(request.args.get("key")) in HP_KEYS
	except:
		return {"Status": "Authentication invalid."}, 403

	request.json.pop("key")

	try:
		assert (e:=Event.query.filter_by(id=request.json["id"])).first() is not None
		e.update(request.json)
		db.session.commit()
		return {"Status": "Event updated."}, 200
	except AssertionError:
		return {"Status": "Event could not be found."}, 400
	except Exception as e:
		return {"Status": "Failure."}, 500


@app.route("/api/hp/event/delete", methods=["POST"])
def delete_event():
	"""
	Delete an event based on its ID.
	"""

	try:
		assert int(request.args.get("key")) in HP_KEYS
	except:
		return {"Status": "Authentication invalid."}, 403

	try:
		assert (e:=Event.query.filter_by(id=request.json["id"])).first() is not None
		e.delete()
		db.session.commit()
		return {"Status": "Event deleted."}, 200
	except AssertionError:
		return {"Status": "Event could not be found."}, 400
	except:
		return {"Status": "Failure."}, 500

@app.route("/api/hp/attendance/create", methods=["POST"])
def create_attendance():
	"""
	Create an attendance for a user.
	"""

	try:
		assert int(request.args.get("key")) in HP_KEYS
	except:
		return {"Status": "Authentication invalid."}, 403

	try:
		user = User.query.filter_by(cardID=request.json["user"]).first()
		event = Event.query.filter_by(id=request.json["event"]).first()
		assert user is not None and event is not None
	except:
		return {"Status": "User or event could not be found,"}, 400

	try:
		assert Attendance.query.filter_by(user=user.id, event=event.id).first() is None
		attendance = Attendance(user.id, event.id)
		db.session.add(attendance)
		db.session.commit()
		return {"Status": "Attendance created."}, 200
	except AssertionError:
		return {"Status": "Attendance already exists."}, 400
	except:
		return {"Status": "Failure."}, 500

