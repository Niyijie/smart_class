package UI;

import Common.*;
import chartPaint.BarChart;
import chartPaint.lineChart;
import chartPaint.pieChart;


import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.DatagramSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class clientUI extends JFrame
{
    private JScrollPane videoArea;    //视频区
    private JPanel buttonArea;   //按钮区
    private JPanel contains;     //视频区容器
    private clientUI self = this;

    private chatUI chat = null;

    public Map<String,ImagePanel> ImapePanelMapPool = null;    //视频线程池

    public Socket clientSock = null;   //客户端tcp套接字

    public static String SERV_IP = "192.168.0.34";

    public static void main(String[] args)
    {
        /*
        * TODO
        * 做成多线程的
        **/

        DatagramSocket sock = null;
        frameGetter getter = null;
        //创建客户端
        clientUI ui = new clientUI();
        //创建套接字流
        InputStream ips = null;
        OutputStream out = null;
//        getter = new frameGetter("rtsp://admin:admin123456@192.168.0.64/Streaming/Channels/1",ui.clientSock);
//        new UiThread(ui, "123", getter).start();

        //tcp 建立连接
        try {
            ui.clientSock = new Socket(clientUI.SERV_IP, 9190);
            //获取套接字流
            ips = ui.clientSock.getInputStream();
            out = ui.clientSock.getOutputStream();

            //向服务器发送客户端登陆消息
            out.write(util.int2Bytes(message.CLIENT_LOGIN_MESSAGE));
            while (true) {
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

        //添加课堂分析按钮
        JButton chartsBtn = new JButton("表图分析");
        chartsBtn.setBounds(10,10,170,90);
        chartsBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFrame frame=new JFrame("教学图表分析");
                frame.setLayout(new GridLayout(2,2));
                frame.add(new BarChart().getChartPanel());           //添加柱形图
                frame.add(new pieChart().getChartPanel());           //添加饼状图
                frame.add(new lineChart().getChartPanel());          //添加折线图
                frame.setBounds(0,0, screenWidth, screenHeight);
                frame.setVisible(true);
            }
        });
        buttonArea.add(chartsBtn);

        //添加考勤分析按钮
        JButton workAnalyseBtn = new JButton("考勤分析");
        workAnalyseBtn.setBounds(10,115,170,90);
        buttonArea.add(workAnalyseBtn);

        //添加不合格记录按钮
        JButton unqualifiedBtn = new JButton("不合格记录");
        unqualifiedBtn.setBounds(10,215,170,90);
        unqualifiedBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                unqualifiedRecordUI ui = new unqualifiedRecordUI();
            }
        });

        buttonArea.add(unqualifiedBtn);

        //添加查找按钮
        JButton searchBtn = new JButton("查找");
        searchBtn.setBounds(10,315,170,90);
        searchBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String room = JOptionPane.showInputDialog("请输入你要查找的教室：\n");
                if(ImapePanelMapPool.containsKey(room)) {
                    ImagePanel panel = ImapePanelMapPool.get(room);
                    if (panel.getClassUI() == null) {
                        classUI ui = new classUI(panel.getter,panel.getClassRoom(),panel.getStuBoxsLocation());
                        panel.setClassUI(ui);
                    } else {
                        panel.getClassUI().setVisible(true);
                    }
                }
            }
        });
        buttonArea.add(searchBtn);

        //添加消息通知按钮
        JButton chatBtn = new JButton("消息通知");
        chatBtn.setBounds(10,415,170,90);
        chatBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (chat == null)
                    chat = new chatUI(self);
                else
                    chat.setVisible(true);
            }
        });
        buttonArea.add(chatBtn);

        //添加刷新按钮
        JButton flushBtn = new JButton("刷新");
        flushBtn.setBounds(10,515,170,90);
        flushBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    OutputStream out = clientSock.getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_RTSP_GET_MESSAGE));
                }
                catch (Exception ex)
                {
                    ex.printStackTrace();
                }

                repaint();
            }
        });
        buttonArea.add(flushBtn);

        //添加退出按钮
        JButton exitBtn = new JButton("退出");
        exitBtn.setBounds(10,615,170,90);
        exitBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });
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
