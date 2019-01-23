package UI;

import Common.util;
import chartPaint.BarChart;
import chartPaint.lineChart;
import chartPaint.pieChart;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;

public class unqualifiedRecordUI extends JFrame{
    private JPanel contains;     //视频区容器
    private JScrollPane videoArea;    //视频区
    private String path = "/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/unquarify/"; // 路径


    private int windowLen;           //窗口的大小

    public unqualifiedRecordUI()
    {
        init();
        this.windowLen = (int)(util.getScreenSize().width - 60 -50) / 3;
        this.setVisible(true);
        readPic();
    }

    private void init()
    {
        this.setTitle("历史记录");
        //设置全屏
        Dimension screenSize = util.getScreenSize();
        int screenWidth = screenSize.width;
        int screenHeight = screenSize.height;
        this.setBounds(0,0, screenWidth, screenHeight);
        //设置屏幕关闭响应
        //this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //设置窗体布局为空布局
        this.setLayout(null);

        //初始化视频区容器
        contains = new JPanel();
        contains.setPreferredSize(new Dimension((int)(screenWidth),2*screenHeight));
        contains.setLayout(null);


        //设置视频监控区
        videoArea = new JScrollPane(contains);
        videoArea.setBounds(0,0,(int)(screenWidth),screenHeight);
        this.getContentPane().add(videoArea);

    }

    private ArrayList<String> getFileName() {
        File f = new File(path);
        if (!f.exists()) {
            System.out.println(path + " not exists");
            return null;
        }

        File fa[] = f.listFiles();
        ArrayList<String> list = new ArrayList<>();
        for (int i = 0; i < fa.length; i++) {
            list.add(fa[i].getName());
        }
        return list;
    }

    private void readPic()
    {
        ArrayList<String> list = getFileName();
        if (list == null)
            return;
        int index = 0;
        for (int i=0;i<list.size();i++) {
            if(".DS_Store".equals(list.get(i)))
                continue;
            else
            {
                JLabel label = new JLabel();
                ImageIcon icon = new ImageIcon(path+list.get(i));
                ImageIcon temp_icon = new ImageIcon();
                temp_icon.setImage(icon.getImage().getScaledInstance(windowLen,windowLen, Image.SCALE_DEFAULT));
                label.setIcon(temp_icon);
                label.addMouseListener(new MouseAdapter() {
                    @Override
                    public void mouseClicked(MouseEvent e) {
                        if(e.getClickCount() >= 2)
                        {
                            picShowUI ui = new picShowUI(icon);
                        }
                    }
                });
                contains.add(label);
                setIconLocation(label,index);
                index++;
                System.out.println(list.get(i));
            }
        }
    }

    //设置框位置
    public void setIconLocation(JLabel lable,int index)
    {
        int col = index % 3;   //框所在列数
        int row = index / 3;   //框所在行数
        int locationX = col * this.windowLen + 22*(col + 1);     //框所在x坐标 22为偏移量
        int locationY = row * this.windowLen + 20*(row + 1);     //框所在y坐标

        //如果位置不需要更新
        if(this.getLocation().x == locationX && this.getLocation().y == locationY)
        {
            return;
        }
        //设置位置
        lable.setBounds(locationX,locationY,windowLen,windowLen);
    }

}
