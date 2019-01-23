package UI;

import Common.frameGetter;
import Common.util;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.net.DatagramSocket;
import java.util.List;

public class picShowUI extends JFrame {
    private JLabel videoArea;    //视频区
    private JPanel infoArea;          //信息区


    private JLabel roomNum;              //房间号标签
    private String classRoom;            //教室号

    private JLabel arrivedNum;           //实到人数标签
    private int arrNum = 0;

    private JLabel listenNum;            //听课人数标签
    private int lstNum = 0;

    private JLabel sleepNum;             //睡觉人数标签
    private int sltNum = 0;

    private JLabel playNum;              //玩手机人数标签
    private int pyNum = 0;

    private JLabel teacherName;          //教师名字标签
    private String tchName = "杨力";

    private JLabel teacherAct;           //教师行为标签
    private String tchAct = "上课";

    private ImageIcon icon = null;

    //设置全屏
    Dimension screenSize = util.getScreenSize();


    picShowUI(ImageIcon icon)
    {
        this.icon = icon;
        this.classRoom = "D204";
        init();
        this.setVisible(true);
    }


    private void init()
    {
        this.setTitle("教室"+classRoom+" 实时分析系统");

        int screenWidth = screenSize.width;
        int screenHeight = screenSize.height;
        this.setBounds(0,0, screenWidth, screenHeight);

        //设置窗体布局为空布局
        this.setLayout(null);

        //设置视频区
        videoArea = new JLabel();
        videoArea.setBounds(0,0,(int)(0.85*screenWidth),screenHeight);
        videoArea.setLayout(null);
        icon.setImage(icon.getImage().getScaledInstance((int)(0.85*screenWidth),screenHeight,Image.SCALE_DEFAULT));
        videoArea.setIcon(icon);

        //设置信息区
        infoArea = new JPanel();
        infoArea.setBounds((int)(0.85*screenWidth),0,(int)(0.15*screenWidth),screenHeight);
        infoArea.setLayout(null);
        //教室号标签
        roomNum = new JLabel();
        roomNum.setBounds(20,50,200,30);
        roomNum.setText("教室: " + classRoom);
        roomNum.setFont(new Font("宋体", Font.PLAIN, 16));
        infoArea.add(roomNum);

        //实到人数标签
        arrivedNum = new JLabel();
        arrivedNum.setBounds(20,100,200,30);
        arrivedNum.setText("实到人数:   " + arrNum);
        arrivedNum.setFont(new Font("宋体", Font.PLAIN, 16));
        infoArea.add(arrivedNum);

        //听课人数标签
        listenNum = new JLabel();
        listenNum.setBounds(20,150,200,30);
        listenNum.setText("听课人数:   " + lstNum);
        listenNum.setFont(new Font("宋体", Font.PLAIN, 16));
        infoArea.add(listenNum);

        //睡觉人数标签
        sleepNum = new JLabel();
        sleepNum.setBounds(20,200,200,30);
        sleepNum.setText("睡觉人数:   " + sltNum);
        sleepNum.setFont(new Font("宋体", Font.PLAIN, 16));
        infoArea.add(sleepNum);

        //玩手机人数
        playNum = new JLabel();
        playNum.setBounds(20,250,200,30);
        playNum.setText("玩手机人数: " + pyNum);
        playNum.setFont(new Font("宋体", Font.PLAIN, 16));
        infoArea.add(playNum);

        //教师姓名
        teacherName = new JLabel();
        teacherName.setBounds(20,300,200,30);
        teacherName.setText("教师姓名:   " + tchName);
        teacherName.setFont(new Font("宋体", Font.PLAIN, 16));
        infoArea.add(teacherName);

        //教师行为
        teacherAct = new JLabel();
        teacherAct.setBounds(20,350,200,30);
        teacherAct.setText("教师行为:   " + tchAct);
        teacherAct.setFont(new Font("宋体", Font.PLAIN, 16));
        infoArea.add(teacherAct);

        this.getContentPane().add(videoArea);
        this.getContentPane().add(infoArea);
    }
}
