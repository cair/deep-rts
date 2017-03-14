
from io import StringIO
import json
from apiclient import discovery
#from oauth2client.service_account import ServiceAccountCredentials
from oauth2client.client import SignedJwtAssertionCredentials
import httplib2
from pydrive.auth import GoogleAuth


import os
def upload(backupFile, backupFileName):


    """Email of the Service Account"""
    SERVICE_ACCOUNT_EMAIL = 'pydrive@phd-drive.iam.gserviceaccount.com'

    """Path to the Service Account's Private Key file"""
    SERVICE_ACCOUNT_PKCS12_FILE_PATH = 'phd-drive.p12'

    f = open(SERVICE_ACCOUNT_PKCS12_FILE_PATH, 'rb')
    key = f.read()
    f.close()

    credentials = SignedJwtAssertionCredentials(SERVICE_ACCOUNT_EMAIL, key,
        scope='https://www.googleapis.com/auth/drive', sub='email')
    http = httplib2.Http()
    credentials.authorize(http)

    gauth = GoogleAuth()
    gauth.credentials = credentials

    drive = GoogleDrive(gauth)

    file1 = drive.CreateFile({'title': backupFileName, "parents" : [{"id":"0B7FoN03AUUdZVlNETEtWLS1VTzQ"}]} )  # Create GoogleDriveFile instance with title 'Hello.txt'

    file1.SetContentFile(backupFile);
    file1.Upload()

upload("test","test")

"""
# from google API console - convert private key to base64 or load from file
id = "pydrive@phd-drive.iam.gserviceaccount.com"
key = base64.b64decode(open("phd-drive.p12", "r").read())

credentials = ServiceAccountCredentials.from_p12_keyfile_buffer(id, StringIO.StringIO(key), scopes='https://www.googleapis.com/auth/drive')
http = credentials.authorize(httplib2.Http())
drive = discovery.build("drive", "v2", http=http)

# notasecret
"""