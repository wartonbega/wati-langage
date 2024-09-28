import json
import os


def __CONFIG_VSCODE_EXTENSION():
    user = os.popen("whoami").read()[:-1]
    extension_path = f"/home/{user}/.vscode/extensions/extensions.json"

    content = ""
    r = None

    with open(extension_path, "r") as file:
        if file == "":
            r = json.load("[]")
        else:
            r = json.load(file)

    r.append({
            "identifier": {
                "id": "AntonAppel.wati"
            },
            "version": "0.0.1",
            "location": {
                "$mid": 1,
                "path": f"/home/{user}/.vscode/extensions/vscode-wati-extension",
                "scheme": "file"
            },
            "relativeLocation": "vscode-wati-extension"
    })

    with open(extension_path, "w") as file:
        file.write(json.dumps(r))

vscode = input("Installer l'extension VS-code [o/N] ?").lower() != "n"
if vscode:
    __CONFIG_VSCODE_EXTENSION()