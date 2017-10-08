def ensure_data_directory():
    import shutil
    import os
    dir_path = os.path.dirname(os.path.realpath(__file__))

    try:
        shutil.copytree(os.path.join(dir_path, "data", os.path.join(os.getcwd(), "data")))
    except:
        print("Data directory already exists or could not be found")
