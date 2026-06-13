import json
import os

# Data directory path — is file ke paas hi data/ folder hai
DATA_DIR = os.path.join(os.path.dirname(__file__), "data")


def load_json(filename):
    """JSON file padhke Python object return karo (list ya dict)."""
    path = os.path.join(DATA_DIR, filename)
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def save_json(filename, data):
    """Python object ko JSON file mein likhdo — existing file overwrite hogi."""
    path = os.path.join(DATA_DIR, filename)
    with open(path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2, ensure_ascii=False)


def get_next_id(items):
    """List of dicts mein sabse bada id dhundho aur +1 return karo.
    Agar list empty hai toh 1 se start karo."""
    if not items:
        return 1
    return max(item["id"] for item in items) + 1
