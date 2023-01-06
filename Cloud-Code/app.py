import json
from flask import Flask, render_template



app = Flask(__name__)


@app.route('/')
def index():  # put application's code here
    return render_template("index.html")

@app.route('/data')
def data():
    with open('data.json', 'r') as file:
        return file

if __name__ == '__main__':
    app.run()
