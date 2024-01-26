function call_Back__sheet(output){
    let chart = anychart.line(output["result"]);
    chart.title(output["title"]);
    chart.container(output["idContainer"]).draw();
}

function call_Back__data_by_materials(output){
    let data = output["result"]
    let seriesData = []
    for(let index =0; index<data.length;index++){
    seriesData.push({
    "x":data[index]["value1"],
    "value":data[index]["value2"]
    })
    }
    let chart = anychart.line(seriesData);
    chart.title(output["title"]);
    chart.container(output["idContainer"]).draw();
}

function compare(a, b) {
  if (a[1]<b[1]) {
    return -1;
  }
  if (a[1]>b[1]) {
    return 1;
  }

  return 0;
}
function call_Back__light_and_debalance_by_materials(output){
    let chart = anychart.column();
    chart.title(output["title"]);
    let seriesData=[]
    let seriesData_2=[]
    for (let i = 0; i < output.dataTable.length; i++) {
        let seriesData=[]
        strdsd=""
        for (let j = 0; j < output.result.length; j++) {
            seriesData.push( [output.result[j]["name"], output.result[j]["value"+(i+1)]])
            strdsd += output.result[j]["value"+(i+1)] + ","
        }
        console.log(strdsd)

        chart.column(seriesData);
    }
    chart.title(output["title"]);
    chart.container(output["idContainer"]).draw();
}

function call_Back__gggggg(output){
    let chart = anychart.scatter();
    chart.title(output["title"]);
    let seriesData=[]
    let seriesData_2=[]
    for (let i = 1; i < output.dataTable.length; i++) {
        let seriesData=[]

        for (let j = 1; j < output.result.length; j++) {
            seriesData.push( [output.result[0]["value"+(i+1)], output.result[j]["value"+(i+1)]])
        }
        var data_1 = seriesData;
        var data_2 = setTheoryData(seriesData);
        var series1 = chart.marker(data_1);
        series1.name("Experimental data");

        // creating the second series (line) and setting the theoretical data
        var series2 = chart.line(data_2);
        series2.name("Theoretically calculated data");
        series2.markers(true);
    }
    chart.title(output["title"]);
    chart.container(output["idContainer"]).draw();
}

anychart.onDocumentLoad(function() {
    eel.getValuesByNameAndIndex({
    nameColumn:"Unnamed: 6",
    indexRow:5,
    title:"График сырья от страницы",
    idContainer:"container_chart__material-in-sheet",
    result:[]
    })(call_Back__sheet)
    eel.getValuesByNameAndIndex({
    nameColumn:"Unnamed: 6",
    indexRow:7,
    title:"График бензина от страницы",
    idContainer:"container_chart__oil-in-sheet",
    result:[]
    })(call_Back__sheet)
    eel.getValuesByNameAndIndex({
    nameColumn:"Unnamed: 6",
    indexRow:10,
    title:"График дизеля от страницы",
    idContainer:"container_chart__diesel-in-sheet",
    result:[]
    })(call_Back__sheet)
    eel.getValuesByNameAndIndex({
    nameColumn:"Unnamed: 6",
    indexRow:12,
    title:"График светлых от страницы",
    idContainer:"container_chart__light-in-sheet",
    result:[]
    })(call_Back__sheet)
    eel.getValuesByNameAndIndex({
    nameColumn:"Unnamed: 6",
    indexRow:24,
    title:"График дебаланс от страницы",
    idContainer:"container_chart__debalance-in-sheet",
    result:[]
    })(call_Back__sheet)

    eel.get_values_by_names_and_indexs({
    dataTable:[{nameColumn:"Unnamed: 6",indexRow:5},{nameColumn:"Unnamed: 6",indexRow:7}],
    title:"График бензина от сырья",
    idContainer:"container_chart__oil-in-material",
    result:[]
    })(call_Back__data_by_materials)
    eel.get_values_by_names_and_indexs({
    dataTable:[{nameColumn:"Unnamed: 6",indexRow:5},{nameColumn:"Unnamed: 6",indexRow:10}],
    title:"График дизиля от сырья",
    idContainer:"container_chart__diesel-in-material",
    result:[]
    })(call_Back__data_by_materials)
    eel.get_values_by_names_and_indexs({
    dataTable:[{nameColumn:"Unnamed: 6",indexRow:5},{nameColumn:"Unnamed: 6",indexRow:12}],
    title:"График светлых от сырья",
    idContainer:"container_chart__light-in-material",
    result:[]
    })(call_Back__data_by_materials)
    eel.get_values_by_names_and_indexs({
    dataTable:[{nameColumn:"Unnamed: 6",indexRow:5},{nameColumn:"Unnamed: 6",indexRow:24}],
    title:"График дебаланс от сырья",
    idContainer:"container_chart__debalance-in-material",
    result:[]
    })(call_Back__data_by_materials)
    eel.get_values_by_names_and_indexs({
    dataTable:[{nameColumn:"Unnamed: 6",indexRow:24}],
    title:"Гистограма относительных частот для  дебаланса",
    idContainer:"container_chart__gisto-debalance-in-material",
    result:[]
    })(call_Back__light_and_debalance_by_materials)
    eel.get_values_by_names_and_indexs({dataTable:[{nameColumn:"Unnamed: 6",indexRow:12}],
    title:"Гистограма относительных частот для светлых",
    idContainer:"container_chart__gisto-light-in-material",
    result:[]
    })(call_Back__light_and_debalance_by_materials)

//    eel.get_values_by_names_and_indexs({
//    dataTable:[
//    {nameColumn:"Unnamed: 6",indexRow:5},
//    {nameColumn:"Unnamed: 6",indexRow:7},
//    {nameColumn:"Unnamed: 6",indexRow:10},
//    {nameColumn:"Unnamed: 6",indexRow:12},
//    {nameColumn:"Unnamed: 6",indexRow:24}
//    ],
//    title:"Регрессионные прямые",
//    idContainer:"gggggg",
//    result:[]
//    })(call_Back__gggggg)

});