from flask import Flask
import os

app = Flask(__name__)

@app.route('/shutdown', methods=['GET'])
def shutdown():
    os.system('shutdown /s /t 1')  # Windows
    # os.system('sudo shutdown -h now')  # Linux
    return 'Shutting down...'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
