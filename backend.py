from flask import Flask,session,jsonify,request
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

@app.route('/mysite/sensordata', methods=['GET', 'POST'])
def sensordata():

    data=request.get_json()
    value1 = data['value1']
    value2 = data['value2']
    value3 = data['value3']
    value4 = data['value4']
    value5 = data['value5']
    value6 = data['value6']
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('INSERT INTO lbtms_datasheet VALUES (NULL,CURRENT_TIMESTAMP, %s, %s,%s,%s,%s,%s)', (value1, value2,value3,value4,value6,value5,))
    mysql.connection.commit()

@app.route('/mysite/sensordataerror', methods=['GET', 'POST'])
def sensordataerror():


    data=request.get_json()
    value1 = data['value1']
    value2 = data['value2']
    value3 = data['value3']
    value4 = data['value4']
    value5 = data['value5']
    value6 = data['value6']
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('INSERT INTO fault_data VALUES (NULL,CURRENT_TIMESTAMP, %s, %s,%s,%s,%s,%s)', (value1, value2,value3,value4,value6,value5,))
    mysql.connection.commit()



@app.route('/mysite/senddata', methods=['GET', 'POST'])
def senddata():
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute("SELECT * FROM lbtms_datasheet ORDER BY id DESC LIMIT 1")
    data = cursor.fetchone()
    cursor.close()
    if data['temperature_cell1'] > 600 or data['temperature_cell2'] > 600 or data['temperature_cell3'] > 600 or data['temperature_cell4'] > 600 or data['Voltage'] > 8 or data['Current'] > 10:
       data['status'] = "Fault detected"
    else:
       data['status'] = "Normal"
    return jsonify(data)

@app.route('/mysite/senddatafault', methods=['GET', 'POST'])
def senddatafault():
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute("SELECT * FROM fault_data ORDER BY id DESC LIMIT 1")
    data = cursor.fetchone()
    cursor.close()
    return jsonify(data)



if __name__ == '__main__':
    app.run(host='0.0.0.0',port=3001)

    app.run(host='0.0.0.0',port=3001)
if __name__ == '__main__':
    app.run(host='0.0.0.0',port=3001)
