package chartPaint;

import java.awt.Font;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.CategoryAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;

public class BarChart {
    private ChartPanel frame1;
    public  BarChart(){
        CategoryDataset dataset = getDataSet();
        JFreeChart chart = ChartFactory.createBarChart3D(
                "实时人数分析图表", // 图表标题
                "实时人数", // 目录轴的显示标签
                "数量", // 数值轴的显示标签
                dataset, // 数据集
                PlotOrientation.VERTICAL, // 图表方向：水平、垂直
                true,           // 是否显示图例(对于简单的柱状图必须是false)
                false,          // 是否生成工具
                false           // 是否生成URL链接
        );

        //解决汉字乱码问题
        CategoryPlot plot=chart.getCategoryPlot();//获取图表区域对象
        CategoryAxis domainAxis=plot.getDomainAxis();         //水平底部列表
        domainAxis.setLabelFont(new Font("黑体",Font.BOLD,14));         //水平底部标题
        domainAxis.setTickLabelFont(new Font("宋体",Font.BOLD,12));  //垂直标题
        ValueAxis rangeAxis=plot.getRangeAxis();//获取柱状
        rangeAxis.setLabelFont(new Font("黑体",Font.BOLD,15));
        chart.getLegend().setItemFont(new Font("黑体", Font.BOLD, 15));
        chart.getTitle().setFont(new Font("宋体",Font.BOLD,20));//设置标题字体

        frame1=new ChartPanel(chart,true); //这里也可以用chartFrame,可以直接生成一个独立的Frame

    }
    private static CategoryDataset getDataSet() {
        DefaultCategoryDataset dataset=new DefaultCategoryDataset();
        dataset.setValue(80,"a","总人数");
        dataset.setValue(60,"b","专心听课人数");
        dataset.setValue(10,"c","不专心听课人数");
        dataset.setValue(10,"d","睡觉人数");
        return dataset;
    }
    public ChartPanel getChartPanel(){
        return frame1;
    }
}