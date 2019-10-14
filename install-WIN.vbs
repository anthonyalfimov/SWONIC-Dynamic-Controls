Option explicit

Dim objShell, objFSO

Set objShell = Wscript.CreateObject("Wscript.Shell")
Set objFSO = CreateObject("Scripting.FileSystemObject")

Dim docsPath, destinationPath, parentPath

docsPath = objShell.SpecialFolders("MyDocuments")
destinationPath = objFSO.BuildPath(docsPath, "ROLI\Littlefoot\")
parentPath = objFSO.GetParentFolderName(WScript.ScriptFullName)

Sub BuildFullPath(ByVal fullPath)
    If Not objFSO.FolderExists(fullPath) Then
        BuildFullPath objFSO.GetParentFolderName(fullPath)
        objFSO.CreateFolder fullPath
    End If
End Sub

BuildFullPath destinationPath

Dim objItem, objRE

Set objRE = New RegExp
objRE.IgnoreCase = True
objRE.Pattern = "\.littlefoot$"

Dim successMsg

For Each objItem In objFSO.GetFolder(parentPath).Files
    If objRE.Test(objItem.name) Then
        objItem.Copy destinationPath, True
        successMsg = successMsg & vbCrLf & objItem.name
    End If
Next

If Len(successMsg) = 0 Then
    MsgBox "ERROR: No `.littlefoot` files found!" & vbCrLf & vbCrLf & "Place `install-WIN.vbs` in the same folder as `.littlefoot` files you want to install and try again!", 0, "Installation failed"

    Wscript.Quit
End If

objRE.Pattern = "\.littlefootModes$"

For Each objItem In objFSO.GetFolder(parentPath).SubFolders
    If objRE.Test(objItem.name) Then
        objItem.Copy destinationPath, True
        successMsg = successMsg & vbCrLf & objItem.name
    End If
Next

successMsg = "Successfully installed:" & successMsg

MsgBox successMsg, 0, "Installation successful"