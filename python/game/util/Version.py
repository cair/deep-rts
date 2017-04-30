import os


def get_version():
    BASE_PATH = os.path.dirname(os.path.realpath(__file__))
    BASE_PATH = os.path.join(BASE_PATH, "../../../", ".git/refs/heads")

    with open(BASE_PATH + '/c++', 'r') as f:
        version = f.read()
        return version.strip()

    return "N/A"