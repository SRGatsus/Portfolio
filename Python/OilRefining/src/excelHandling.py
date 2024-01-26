import pandas as pd


class ExcelHandling():
    filePath = None
    xl = None

    def __init__(self, filePath):
        self.filePath = filePath
        self.xl = pd.ExcelFile(filePath)

    def getArraySheetNames(self):
        return self.xl.sheet_names

    def getValuesByNameAndIndex(self, nameColumn, indexRow):
        result = []
        for name in self.getArraySheetNames():
            sheet = self.xl.parse(name)
            result.append({
                "name": str(name).replace("Лист", ""),
                "value": sheet[nameColumn][indexRow]
            })
        return result

    def getDependenceValuesByNameAndIndex(self, data, is_object=True):
        result = []
        for name in self.getArraySheetNames():
            sheet = self.xl.parse(name)
            if is_object:
                element = {
                    "name": str(name).replace("Лист", ""),
                }
            else:
                element = []
            index = 1
            for x in data:
                if is_object:
                    element["value" + str(index)] = sheet[x["nameColumn"]][x["indexRow"]]
                else:
                    element.append(sheet[x["nameColumn"]][x["indexRow"]])
                index += 1
            result.append(element)
        return result
