from flask import Flask

app = Flask(__name__)


@app.get("/")
def index():
    return "University Result Portal"


@app.get("/health")
def health():
    return {"status": "ok"}
