from app import db

class User(db.Model):
	"""
	A definition for a single user, consisting of a card ID and points.
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

