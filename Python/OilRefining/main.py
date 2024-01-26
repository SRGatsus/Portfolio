import eel

from src.excelHandling import ExcelHandling

file = 'source/АВТ_2.xlsx'
EHandling = ExcelHandling(file)


@eel.expose
def getValuesByNameAndIndex(data):
    values = EHandling.getValuesByNameAndIndex(data["nameColumn"], data["indexRow"])
    data["result"] = []
    for value in values:
        data["result"].append({
            "x": value["name"],
            "value": value["value"]
        })
    return data


@eel.expose
def get_values_by_names_and_indexs(data):
    data["result"] = EHandling.getDependenceValuesByNameAndIndex(data["dataTable"])
    return data


if __name__ == '__main__':
    eel.init('web')
    eel.start('main.html', size=(700, 700))
