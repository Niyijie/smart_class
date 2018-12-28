package UI;

import Common.*;


import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.DatagramSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class clientUI extends JFrame
{
    private JScrollPane videoArea;    //视频区
    private JPanel buttonArea;   //按钮区
    private JPanel contains;     //视频区容器

    public Map<String,ImagePanel> ImapePanelMapPool = null;    //视频线程池

    public Socket clientSock = null;   //客户端tcp套接字




    public static void main(String[] args)
    {
        /*
        * TODO
        * 做成多线程的
        * */

        DatagramSocket sock = null;
        frameGetter getter = null;
        //创建客户端
        clientUI ui = new clientUI();
        //创建套接字流
        InputStream ips = null;
        OutputStream out = null;
        //tcp 建立连接
        try {
            ui.clientSock = new Socket("172.20.10.14", 9190);
            //获取套接字流
            ips = ui.clientSock.getInputStream();
            out = ui.clientSock.getOutputStream();

            //向服务器发送客户端登陆消息
            out.write(util.int2Bytes(message.PI_LOGIN_MESSAGE));
            while (true) {
                //获取摄像头端登陆消息
                byte[] msgBytes = new byte[4];
                ips.read(msgBytes);
                int type = util.bytes2Int(msgBytes);
                messageParseFactory.parseFactory(ui,type);
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

    }

    clientUI()
    {
        init();
        this.setVisible(true);
    }

    /*
    * 初始化界面
    * */
    private void init()
    {
        this.setTitle("后台监控系统");
        //初始化线程池和map
        ImapePanelMapPool = new HashMap<>();
        //设置全屏
        Dimension screenSize = util.getScreenSize();
        int screenWidth = screenSize.width;
        int screenHeight = screenSize.height;
        this.setBounds(0,0, screenWidth, screenHeight);
        //设置屏幕关闭响应
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //设置窗体布局为空布局
        this.setLayout(null);

        //初始化视频区容器
        contains = new JPanel();
        contains.setPreferredSize(new Dimension((int)(0.85*screenWidth),2*screenHeight));
        contains.setLayout(null);

        //设置视频监控区
        videoArea = new JScrollPane(contains);
        videoArea.setBounds(0,0,(int)(0.85*screenWidth),screenHeight);

        //设置按钮区
        buttonArea = new JPanel();
        buttonArea.setLayout(null);
        buttonArea.setBounds((int)(0.85*screenWidth),0,(int)(0.15*screenWidth),screenHeight);
        //buttonArea.setBackground(Color.red);

        //添加课堂分析按钮
        JButton classAnalyseBtn = new JButton("课堂分析");
        classAnalyseBtn.setBounds(10,10,170,90);
        buttonArea.add(classAnalyseBtn);

        //添加教师分析按钮
        JButton teacherAnalyseBtn = new JButton("教师分析");
        teacherAnalyseBtn.setBounds(10,115,170,90);
        buttonArea.add(teacherAnalyseBtn);

        //添加考勤分析按钮
        JButton workAnalyseBtn = new JButton("考勤分析");
        workAnalyseBtn.setBounds(10,215,170,90);
        buttonArea.add(workAnalyseBtn);

        //添加单通知按钮
        JButton oneWayTalkBtn = new JButton("单通知");
        oneWayTalkBtn.setBounds(10,315,170,90);
        buttonArea.add(oneWayTalkBtn);

        //添加群通知按钮
        JButton groupWayTalkBtn = new JButton("群通知");
        groupWayTalkBtn.setBounds(10,415,170,90);
        buttonArea.add(groupWayTalkBtn);

        //添加刷新按钮
        JButton flushBtn = new JButton("刷新");
        flushBtn.setBounds(10,515,170,90);
        buttonArea.add(flushBtn);

        //添加退出按钮
        JButton exitBtn = new JButton("退出");
        exitBtn.setBounds(10,615,170,90);
        buttonArea.add(exitBtn);

        //添加到框架中
        this.getContentPane().add(videoArea);
        this.getContentPane().add(buttonArea);
    }

    //动态添加监控区
    public void addImagePanel(String _room,frameGetter getter)
    {
        int index = ImapePanelMapPool.size();
        ImagePanel panel = new ImagePanel(getter,_room,index);
        ImapePanelMapPool.put(_room,panel);
        contains.add(panel);
        repaint();
    }

}
