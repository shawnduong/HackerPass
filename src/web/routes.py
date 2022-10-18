from app import *

@app.route("/", methods=["GET"])
def index():
	"""
	Display the index page to the user.
	"""

	return render_template("index.html")

