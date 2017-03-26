## Simple Python module to upload files to Google Drive
# Needs a file 'client_secrets.json' in the directory 
# The file can be obtained from https://console.developers.google.com/ 
# under APIs&Auth/Credentials/Create Client ID for native application

# To test usage:
# import google_drive_util
# google_drive_util.login()
# google_drive_util.test()


import os
from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive


def login():
    global gauth, drive
    gauth = GoogleAuth()
    gauth.LocalWebserverAuth() # Creates local webserver and auto handles authentication
    drive = GoogleDrive(gauth) # Create GoogleDrive instance with authenticated GoogleAuth instance

def root_files():    
    file_list = drive.ListFile({'q': "'root' in parents and trashed=false"}).GetList()
    # Auto-iterate through all files in the root folder.
    #for file1 in file_list:
    #    print 'title: %s, id: %s' % (file1['title'], file1['id'])
    return file_list


def find_folders(fldname):
    file_list = drive.ListFile({
        'q': "title='{}' and mimeType contains 'application/vnd.google-apps.folder' and trashed=false".format(fldname)
        }).GetList()
    return file_list


def create_subfolder(folder,sfldname):
    new_folder = drive.CreateFile({'title':'{}'.format(sfldname),       
                               'mimeType':'application/vnd.google-apps.folder'})
    if folder is not None:
        new_folder['parents'] = [{u'id': folder['id']}]
    new_folder.Upload()
    return new_folder


def list_files_with_ext(ext,dir='./'):
    return sorted(filter(lambda f:f[-len(ext):]==ext,os.listdir(dir)))


def upload_files_to_folder(fnames, folder):
    #file1 = drive.CreateFile({'title': 'Hello.txt'}) # Create GoogleDriveFile instance with title 'Hello.txt'
    #file1.Upload() # Upload it
    #print 'title: %s, id: %s' % (file1['title'], file1['id']) # title: Hello.txt, id: {{FILE_ID}}
    
    for fname in fnames: 
        nfile = drive.CreateFile({'title':os.path.basename(fname),
                                  'parents':[{u'id': folder['id']}]})
        nfile.SetContentFile(fname)
        nfile.Upload() 

def test():
    #folder = find_folders('test')[0]
    new_folder = create_subfolder(None,'testfolder')
    upload_files_to_folder(list_files_with_ext('.png')[:3],new_folder)
    return new_folder

login()
test()