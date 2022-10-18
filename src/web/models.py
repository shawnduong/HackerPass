from __future__ import annotations

from app import db

from flask_login import UserMixin
from typing import Union

class User(UserMixin, db.Model):
	"""
	A definition for a single user, consisting of a unique card ID and points.
	"""

	__tablename__ = "user"

	id      = db.Column(db.Integer, primary_key=True)
	cardID  = db.Column(db.Integer    , unique=True , nullable=False)
	name    = db.Column(db.String(256), unique=False, nullable=True )
	email   = db.Column(db.String(256), unique=False, nullable=True )
	points  = db.Column(db.Integer    , unique=False, nullable=False)

	def __init__(self, cardID=0, name=None, email=None, points=0):
		"""
		Constructor method for User type objects.
		"""

		self.cardID  = cardID
		self.name    = name
		self.email   = email
		self.points  = points

	def login(cardID: str) -> Union[User, bool]:
		"""
		Check if a cardID string is valid and return the User if so. Else,
		return False.
		"""

		# Convert the cardID string from hex to an integer.
		try:
			intCardID = int(cardID, 16)
		except:
			return False

		if ((user:=User.query.filter_by(cardID=intCardID).first()) != None):
			return user

		return False

	def update_points(self):
		"""
		Update the user's points as the sum of their attendances.
		"""

		self.points = 0
		attendances = Attendance.query.filter_by(user=self.id).all()

		for attendance in attendances:
			event = Event.query.filter_by(id=attendance.event).first()
			self.points += event.points

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

