package UI;

import Common.frameGetter;
import Common.message;
import Common.util;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.OutputStream;
import java.net.DatagramSocket;
import java.util.List;

public class classUI extends JFrame {

    private frameGetter getter;      //视频获取器

    private JLabel videoArea;    //视频区
    private JPanel infoArea;          //信息区

    private boolean flag;        //标记是否已经拥有该窗口对象

    private JLabel roomNum;              //房间号标签
    private String classRoom;         //教室号

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

    private JButton classCallBtn;        //课堂点名按钮
    private JButton leftBtn;            //左按钮
    private JButton rightBtn;            //右按钮
    private JButton zoomInBtn;            //放大按钮
    private JButton zoomOutBtn;            //缩小按钮

    private JButton exitBtn;             //退出按钮

    private classUI self;

    private List<Integer> stuBoxsLocation = null;

    //设置全屏
    Dimension screenSize = util.getScreenSize();

    public boolean getFlag()
    {
        return  flag;
    }


    classUI(frameGetter _getter,String _room,List<Integer> _stuBoxLocation)
    {
        flag = true;
        self = this;
        this.getter = _getter;
        this.classRoom = _room;
        this.stuBoxsLocation = _stuBoxLocation;
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
        ImageIcon icon = new ImageIcon("/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/next.jpg");
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

        //课堂点名按钮
        classCallBtn = new JButton("课堂点名");
        classCallBtn.setBounds(20,400,150,90);
        infoArea.add(classCallBtn);

        //控制按钮
        leftBtn = new JButton("向左");
        leftBtn.setBounds(20,500,80,60);
        leftBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent ev) {
                super.mousePressed(ev);

                try {
                    OutputStream out = getter.getClientSock().getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_CONTROL_MESSAGE));
                    out.write(util.int2Bytes(classRoom.getBytes().length));
                    out.write(classRoom.getBytes());
                    out.write(util.int2Bytes(message.CLIENT_PAN_LEFT_MESSAGE));
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }

                System.out.println("按下");
            }

            @Override
            public void mouseReleased(MouseEvent ev)
            {
                super.mouseReleased(ev);
                try {
                    OutputStream out = getter.getClientSock().getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_CONTROL_MESSAGE));
                    out.write(util.int2Bytes(classRoom.getBytes().length));
                    out.write(classRoom.getBytes());
                    out.write(util.int2Bytes(message.CLIENT_LEFT_STOP_MESSAGE));
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
                System.out.println("松开");
            }

        });
        infoArea.add(leftBtn);

        rightBtn = new JButton("向右");
        rightBtn.setBounds(100,500,80,60);
        rightBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent ev) {
                super.mousePressed(ev);

                try {
                    OutputStream out = getter.getClientSock().getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_CONTROL_MESSAGE));
                    out.write(util.int2Bytes(classRoom.getBytes().length));
                    out.write(classRoom.getBytes());
                    out.write(util.int2Bytes(message.CLIENT_PAN_RIGHT_MESSAGE));
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }

                System.out.println("按下");
            }

            @Override
            public void mouseReleased(MouseEvent ev)
            {
                super.mouseReleased(ev);
                try {
                    OutputStream out = getter.getClientSock().getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_CONTROL_MESSAGE));
                    out.write(util.int2Bytes(classRoom.getBytes().length));
                    out.write(classRoom.getBytes());
                    out.write(util.int2Bytes(message.CLIENT_RIGHT_STOP_MESSAGE));
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
                System.out.println("松开");
            }

        });
        infoArea.add(rightBtn);

        zoomInBtn = new JButton("放大");
        zoomInBtn.setBounds(20,560,80,60);
        zoomInBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent ev) {
                super.mousePressed(ev);

                try {
                    OutputStream out = getter.getClientSock().getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_CONTROL_MESSAGE));
                    out.write(util.int2Bytes(classRoom.getBytes().length));
                    out.write(classRoom.getBytes());
                    out.write(util.int2Bytes(message.CLIENT_ZOOM_IN_MESSAGE));
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }

                System.out.println("按下");
            }

            @Override
            public void mouseReleased(MouseEvent ev)
            {
                super.mouseReleased(ev);
                try {
                    OutputStream out = getter.getClientSock().getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_CONTROL_MESSAGE));
                    out.write(util.int2Bytes(classRoom.getBytes().length));
                    out.write(classRoom.getBytes());
                    out.write(util.int2Bytes(message.CLIENT_ZOOM_IN_STOP_MESSAGE));
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
                System.out.println("松开");
            }

        });
        infoArea.add(zoomInBtn);

        zoomOutBtn = new JButton("缩小");
        zoomOutBtn.setBounds(100,560,80,60);
        zoomOutBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent ev) {
                super.mousePressed(ev);

                try {
                    OutputStream out = getter.getClientSock().getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_CONTROL_MESSAGE));
                    out.write(util.int2Bytes(classRoom.getBytes().length));
                    out.write(classRoom.getBytes());
                    out.write(util.int2Bytes(message.CLIENT_ZOOM_OUT_MESSAGE));
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }

                System.out.println("按下");
            }

            @Override
            public void mouseReleased(MouseEvent ev)
            {
                super.mouseReleased(ev);
                try {
                    OutputStream out = getter.getClientSock().getOutputStream();
                    out.write(util.int2Bytes(message.CLIENT_CONTROL_MESSAGE));
                    out.write(util.int2Bytes(classRoom.getBytes().length));
                    out.write(classRoom.getBytes());
                    out.write(util.int2Bytes(message.CLIENT_ZOOM_OUT_STOP_MESSAGE));
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
                System.out.println("松开");
            }
        });
        infoArea.add(zoomOutBtn);

        //退出按钮
        exitBtn = new JButton("退出");
        exitBtn.setBounds(20,630,150,90);
        exitBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*
                *   TODO
                *   为考虑提高性能 以后可以析构掉，或者控制frameGerrer不再更新图像
                */

                System.out.println("close");
                self.setVisible(false);

            }
        });
        infoArea.add(exitBtn);

        this.getContentPane().add(videoArea);
        this.getContentPane().add(infoArea);
    }

    //更新图标
    public void updateIcon(BufferedImage image) {
        /*
        *  TO DO 设置显示框的时间时间
        */
        try {
            Graphics g = image.getGraphics();
            g.setColor(Color.BLUE);
            //显示学生坐标
            if (stuBoxsLocation.size() != 0) {
                for (int i = 0; i < stuBoxsLocation.size(); i += 4)
                    g.drawRect(stuBoxsLocation.get(i), stuBoxsLocation.get(i + 1), stuBoxsLocation.get(i + 2), stuBoxsLocation.get(i + 3));
            }
            ImageIcon icon = new ImageIcon(image);
            //图标缩放知适合大小，并展示
            //icon.setImage(icon.getImage().getScaledInstance((int) (0.85 * screenSize.width), screenSize.height, Image.SCALE_DEFAULT));
            videoArea.setIcon(icon);
        }catch (Exception e)
        {
            e.printStackTrace();
        }
    }


    //更新实到人数标签
    public void updateArrivedStudentLabel(int _num)
    {
        if(arrNum != _num)
        {
            arrivedNum.setText("实到人数:  " + _num);
            arrNum = _num;
        }
    }

}
