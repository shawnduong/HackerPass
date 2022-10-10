from app import db

class User(db.Model):
	"""
	A definition for a single user, consisting of a unique card ID and points.
	"""

	__tablename__ = "user"

	id      = db.Column(db.Integer, primary_key=True)
	cardID  = db.Column(db.Integer, unique=True , nullable=False)
	points  = db.Column(db.Integer, unique=False, nullable=False)

	def __init__(self, cardID=0, points=0):
		"""
		Constructor method for User type objects.
		"""

		self.cardID = cardID
		self.points = points

class Event(db.Model):
	"""
	A definition for a single event, consisting of a unique event ID (id),
	points reward, a title, about, room, and author(s).
	"""

	__tablename__ = "event"

	id      = db.Column(db.Integer, primary_key=True)
	points  = db.Column(db.Integer    , unique=False, nullable=False)
	title   = db.Column(db.String(256), unique=False, nullable=False)
	about   = db.Column(db.String(256), unique=False, nullable=False)
	room    = db.Column(db.String(256), unique=False, nullable=False)
	author  = db.Column(db.String(256), unique=False, nullable=False)

	def __init__(self, points=0, title="", about="", room="", author=""):
		"""
		Constructor method for Event type objects.
		"""

		self.points  = points
		self.title   = title
		self.about   = about
		self.room    = room
		self.author  = author

class Attendance(db.Model):
	"""
	A definition for a single attendance, relating a User to an Event.
	"""

	__tablename__ = "attendance"

	id     = db.Column(db.Integer, primary_key=True)
	user   = db.Column(db.Integer, db.ForeignKey(User.id) , unique=False, nullable=False)
	event  = db.Column(db.Integer, db.ForeignKey(Event.id), unique=False, nullable=False)

	def __init__(self, user=0, event=0):
		"""
		Constructor method for Attendance type objects.
		"""

		self.user  = user
		self.event = event

