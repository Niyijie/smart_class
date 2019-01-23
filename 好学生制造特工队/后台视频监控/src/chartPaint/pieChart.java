package chartPaint;

import java.awt.Font;
import java.text.DecimalFormat;
import java.text.NumberFormat;

import javax.swing.JPanel;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.labels.StandardPieSectionLabelGenerator;
import org.jfree.chart.plot.PiePlot;
import org.jfree.data.general.DefaultPieDataset;

public class pieChart {
    private ChartPanel frame1;
    public pieChart(){
        DefaultPieDataset data = getDataSet();
        JFreeChart chart = ChartFactory.createPieChart3D("课堂质量状态分布", data, true, true, false);
        //设置百分比
        PiePlot pieplot = (PiePlot) chart.getPlot();
        DecimalFormat df = new DecimalFormat("0.00%");//获得一个DecimalFormat对象，主要是设置小数问题
        NumberFormat nf = NumberFormat.getNumberInstance();//获得一个NumberFormat对象
        StandardPieSectionLabelGenerator sp1 = new StandardPieSectionLabelGenerator("{0}  {2}", nf, df);//获得StandardPieSectionLabelGenerator对象
        pieplot.setLabelGenerator(sp1);//设置饼图显示百分比

        // 底部图例显示百分比:自定义方式， {0} 表示选项， {1} 表示数值， {2} 表示所占比例
        pieplot.setLegendLabelGenerator(new StandardPieSectionLabelGenerator("{0}={1}({2})"));
        // 指定显示的饼图上圆形(false)还椭圆形(true) 默认为true
        pieplot.setCircular(true);
        //设置饼图标签字体
        pieplot.setLabelFont(new Font("宋体", Font.PLAIN, 15));

        //没有数据的时候显示的内容
        pieplot.setNoDataMessage("无数据显示");
        pieplot.setCircular(false);
        pieplot.setLabelGap(0.02D);

        pieplot.setIgnoreNullValues(true);//设置不显示空值
        pieplot.setIgnoreZeroValues(true);//设置不显示负值
        frame1=new ChartPanel (chart,true);
        chart.getTitle().setFont(new Font("宋体",Font.BOLD,20));//设置标题字体
        PiePlot piePlot= (PiePlot) chart.getPlot();//获取图表区域对象
        piePlot.setLabelFont(new Font("宋体",Font.BOLD,10));//解决乱码
        chart.getLegend().setItemFont(new Font("黑体",Font.BOLD,10));
    }
    private static DefaultPieDataset getDataSet() {
        DefaultPieDataset mDataset=new DefaultPieDataset();
        mDataset.setValue(" 优秀(0.9~1.0)", new Double(5));
        mDataset.setValue(" 良好(0.7~0.9)", new Double(8));
        mDataset.setValue(" 普通(0.5~0.7)", new Double(2));
        mDataset.setValue(" 差(0.0~0.5)", new Double(5));
        return mDataset;
    }
    public ChartPanel getChartPanel(){
        return frame1;

    }
}