import QtQuick 2.0
import QtQuick.Window 2.2
import QtCharts 2.2
//import DiabChart 1.0

Window {
    visible: true
    width: 640
    height: 480

    ChartView {
        title: "Line"
        width: 640
        height: 480
        anchors.fill: parent
        antialiasing: true
        //function setFin(string) { console.log('got some Items'); }
//        LineSeries: TelecomunicationChart.getSeries()
/*        function addSeries()
        {
            //Define Axes of the ChartView
            diabetesView.getAxisYDescription(yDescription);
            diabetesView.getAxisXTime(xTime);
            diabetesView.getAxisYValues(yValues);

            // Create new LineSeries with 3 Axes (Two-Y-Axis, One-X-Axis)
            var mySeries = diabChartBig.createSeries(ChartView.SeriesTypeLine, "Line", xTime, yValues);
            var mySeries2 = diabChartBig.createSeries(ChartView.SeriesTypeLine, "Overview", xTime, yDescription);

            // Define series on specific wishes
            diabetesView.setLineSeries(mySeries);

            //Delete not needed series (only created because second y-Axis
            diabChartBig.removeSeries(mySeries2);
        }*/

    }
}
