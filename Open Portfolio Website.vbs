Option Explicit
Dim fso, shell, folder, pythonw, candidate, candidates, i, started, req, url, logPath
Set fso = CreateObject("Scripting.FileSystemObject")
Set shell = CreateObject("WScript.Shell")
folder = fso.GetParentFolderName(WScript.ScriptFullName)
url = "http://127.0.0.1:8000"
logPath = fso.BuildPath(folder, "website.log")

' Clear old log so startup errors are easy to find
If fso.FileExists(logPath) Then fso.DeleteFile logPath, True

' Find pythonw.exe directly in PATH (no command window is opened)
pythonw = ""
candidates = Split(shell.ExpandEnvironmentStrings("%PATH%"), ";")
For Each candidate In candidates
    candidate = Replace(candidate, Chr(34), "")
    candidate = Trim(candidate)
    If candidate <> "" Then
        If fso.FileExists(fso.BuildPath(candidate, "pythonw.exe")) Then
            pythonw = fso.BuildPath(candidate, "pythonw.exe")
            Exit For
        End If
    End If
Next

If pythonw = "" Then
    MsgBox "Python was not found on PATH." & vbCrLf & vbCrLf & _
           "Install Python 3 and enable 'Add Python to PATH', then try again." & vbCrLf & _
           "This launcher starts the backend invisibly; no terminal window is needed.", _
           vbExclamation, "Portfolio Website"
    WScript.Quit 1
End If

' Start the backend invisibly; it runs the real Python -> TCP -> C++ pipeline
shell.CurrentDirectory = folder
shell.Run Chr(34) & pythonw & Chr(34) & " " & Chr(34) & fso.BuildPath(folder, "web_server.py") & Chr(34), 0, False

' Wait for the server to answer before opening the browser
started = False
For i = 1 To 40
    WScript.Sleep 500
    On Error Resume Next
    Set req = CreateObject("WinHttp.WinHttpRequest.5.1")
    req.SetTimeouts 300, 300, 300, 300
    req.Open "GET", url, False
    req.Send
    If Err.Number = 0 Then
        If req.Status = 200 Then started = True
    End If
    Err.Clear
    On Error GoTo 0
    If started Then Exit For
Next

If started Then
    shell.Run url, 1, False
Else
    Dim details
    details = "The backend did not start. Python and g++ (C++ compiler) must be installed." & vbCrLf & _
              "The website startup log is: " & logPath & vbCrLf & vbCrLf & _
              "Open the log file to see the exact error."
    If fso.FileExists(logPath) Then
        shell.Run "notepad.exe " & Chr(34) & logPath & Chr(34), 1, False
    Else
        MsgBox details, vbExclamation, "Portfolio Website"
    End If
End If
