package chartPaint;

import java.awt.Font;
import java.text.SimpleDateFormat;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.time.Day;
import org.jfree.data.time.Month;
import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;
import org.jfree.data.xy.XYDataset;

public class lineChart{
    private ChartPanel frame1;
    public lineChart(){
        XYDataset xydataset = createDataset();
        JFreeChart jfreechart = ChartFactory.createTimeSeriesChart("近七天平均课堂质量分析", "日期", "教学质量",xydataset, true, true, true);
        XYPlot xyplot = (XYPlot) jfreechart.getPlot();
        DateAxis dateaxis = (DateAxis) xyplot.getDomainAxis();
        dateaxis.setDateFormatOverride(new SimpleDateFormat("MM-dd"));
        frame1=new ChartPanel(jfreechart,true);
        dateaxis.setLabelFont(new Font("黑体",Font.BOLD,14));         //水平底部标题
        dateaxis.setTickLabelFont(new Font("宋体",Font.BOLD,12));  //垂直标题
        ValueAxis rangeAxis=xyplot.getRangeAxis();//获取柱状
        rangeAxis.setLabelFont(new Font("黑体",Font.BOLD,15));
        jfreechart.getLegend().setItemFont(new Font("黑体", Font.BOLD, 15));
        jfreechart.getTitle().setFont(new Font("宋体",Font.BOLD,20));//设置标题字体

    }
    private static XYDataset createDataset() {  //这个数据集有点多，但都不难理解
        TimeSeries timeseries = new TimeSeries("近七天平均课堂质量分析",
                org.jfree.data.time.Day.class);

        timeseries.add(new Day(1,6,2018), 0.8D);
        timeseries.add(new Day(2,6,2018), 0.7D);
        timeseries.add(new Day(3,6,2018), 0.75D);
        timeseries.add(new Day(4,6,2018), 0.72D);
        timeseries.add(new Day(5,6,2018), 0.75D);
        timeseries.add(new Day(6,6,2018), 0.78D);
        timeseries.add(new Day(7,6,2018), 0.82D);

        TimeSeriesCollection timeseriescollection = new TimeSeriesCollection();
        timeseriescollection.addSeries(timeseries);
        return timeseriescollection;
    }
    public ChartPanel getChartPanel(){
        return frame1;

    }
}