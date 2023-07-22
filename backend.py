from flask import Flask, request, redirect, render_template, url_for, session,jsonify
from flask_session import Session
from flask_cors import CORS
from flask_httpauth import HTTPBasicAuth
from flask_mysqldb import MySQL
import MySQLdb.cursors



app = Flask(__name__)
CORS(app)   # So that the frontend and backend can make requests with necessary permissions
# auth = HTTPBasicAuth()

# Setting Up DB
app.config['MYSQL_HOST'] = 'oenlbtms.mysql.pythonanywhere-services.com'
app.config['MYSQL_USER'] = 'oenlbtms'
app.config['MYSQL_PASSWORD'] = 'insaneinsane'
app.config['MYSQL_DB'] = 'oenlbtms$sensordatas'
mysql = MySQL(app)

app.secret_key = '\xcb\x9e\x84(#/\t\xf74\xfd\x10\x06~2.\xe7\xed\x90hGNNX\xc7'

app.config["SECRET_KEY"] = 'DOTHACK2022'
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
app.config.update(SESSION_COOKIE_SAMESITE="None", SESSION_COOKIE_SECURE=True)
Session(app)



@app.route('/')
def hello_world():
    return 'iyg'






@app.route('/mysite/login', methods=['GET', 'POST'])
def login():
    data=request.get_json()
    username = data['username']
    password = data['password']

    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('SELECT * FROM accounts WHERE username = %s AND password = %s', (username, password,))
    account = cursor.fetchone()
    if account:
            session['loggedin'] = True
            session['password'] = account['password']
            session['username'] = account['username']
            return 5
    else:
            return 'unsuccessfull!'






@app.route('/mysite/register', methods=['GET', 'POST'])
def register():

    data=request.get_json()


    username = data['username']
    password = data['password']
    email = data['email']

    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('INSERT INTO accounts VALUES (NULL, %s, %s, %s)', (username, password, email,))
    mysql.connection.commit()






@app.route('/mysite/sensordata', methods=['GET', 'POST'])
def sensordata():



    data=request.get_json()
    value1 = data['value1']
    value2 = data['value2']
    value3 = data['value3']
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('INSERT INTO sensor_data VALUES (NULL, %s, %s,%s,CURRENT_TIMESTAMP)', (value1, value2,value3,))
    mysql.connection.commit()







@app.route('/mysite/senddata', methods=['GET', 'POST'])
def senddata():




    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute("SELECT * FROM sensor_data ORDER BY id DESC LIMIT 1")


    data = cursor.fetchone()


    cursor.close()
    return jsonify(data)


























if __name__ == '__main__':
    app.run(host='0.0.0.0',port=3001)

    app.run(host='0.0.0.0',port=3001)
if __name__ == '__main__':
    app.run(host='0.0.0.0',port=3001)
