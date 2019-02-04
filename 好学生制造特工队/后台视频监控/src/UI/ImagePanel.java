package UI;

import Common.frameGetter;
import Common.util;
import com.sun.xml.internal.messaging.saaj.util.ByteInputStream;
import sun.plugin.dom.css.Rect;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.Queue;

public class ImagePanel extends JPanel
{
    private JLabel lblIcon;          //教室图像
    private int windowLen;           //窗口的大小
    private String classRoom;        //班级号
    private int studentNum;          //教室里学生人数
    private double quarity;          //课堂质量

    public frameGetter getter;      //视频获取器

    private JLabel Jroom;            //教室号标签
    private JLabel Jquarity;         //课堂质量标签
    private JLabel JstuNum;          //学生数量标签
    private classUI singleUI;

    public boolean flag = false;

    public Queue<Integer> tempNum = new ArrayDeque<>();

    private List<Integer> stuBoxsLocation = null;

    ImagePanel(frameGetter _getter, String _classRoom, int index)
    {

        //记录框的位置信息
        stuBoxsLocation = new ArrayList<>();

        //视频获取类
        this.getter = _getter;
        //panel风格为空
        this.setLayout(null);
        //教室名字
        this.classRoom = _classRoom;
        //初始化为0
        this.studentNum = 0;
        this.quarity = 0;
        //获取计算大小
        this.windowLen = (int)(0.85* util.getScreenSize().width - 60 - 50) / 3;

        //设置框位置
        setIconLocation(index);
        this.setBorder(BorderFactory.createLineBorder(Color.black));

        //添加一个视频播放标签
        lblIcon = new JLabel();
        lblIcon.setPreferredSize(new Dimension(this.windowLen, this.windowLen));
        lblIcon.setBounds(0,0,this.windowLen,this.windowLen);
        lblIcon.setBorder(BorderFactory.createLineBorder(Color.black));
        lblIcon.setIcon(new ImageIcon("/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/logo.png"));

        //图标缩放知适合大小，并展示
        ImageIcon icon = new ImageIcon("/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/logo.png");
        icon.setImage(icon.getImage().getScaledInstance(this.windowLen,this.windowLen,Image.SCALE_DEFAULT));
        lblIcon.setIcon(icon);

        //在视频播放区上添加鼠标监听器
        lblIcon.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if(e.getClickCount() >= 2)
                {
                    //如果第一次打开 则实例化
                    if(singleUI == null) {
                        singleUI = new classUI(getter,classRoom,stuBoxsLocation);
                    }
                    else  //如果已经实例化 则直接显示
                    {
                        singleUI.setVisible(true);
                    }
                }
            }
        });

        Jroom = new JLabel();
        Jroom.setSize(100,30);
        Jroom.setLocation(10,5);
        Jroom.setText("教室号:  " + this.classRoom);
        lblIcon.add(Jroom);

        Jquarity = new JLabel();
        Jquarity.setSize(100,30);
        Jquarity.setLocation(10,25);
        Jquarity.setText("课堂质量:  "+this.quarity);
        lblIcon.add(Jquarity);

        JstuNum = new JLabel();
        JstuNum.setSize(100,30);
        JstuNum.setLocation(10,45);
        JstuNum.setText("学生数量:  "+this.studentNum);
        lblIcon.add(JstuNum);

        this.add(lblIcon);
        this.setVisible(true);
    }

    public classUI getClassUI() { return singleUI; }

    public void setClassUI(classUI ui){ this.singleUI = ui;}

    public List<Integer> getStuBoxsLocation(){return stuBoxsLocation;}

    public void setClassRoom(String _classRoom)
    {
        this.classRoom = _classRoom;
    }

    public String getClassRoom() { return this.classRoom; }

    public void setStudentNum(int _studentNum)
    {
        this.studentNum = _studentNum;
    }

    /*
    * TODO
    * 课堂质量评估函数
    * */
    public void setQuarity(int sleep,int listen,int play,int total)
    {
        //计算质量
        this.quarity = (double) listen / total;
        //更新
        updateQuarityLabel(this.quarity);
    }

    //更新图标
    public void updateIcon(BufferedImage imageData) {
        /*
        *   TO DO 提高性能将imagepanel和classUi的icon用同一个引用
        */
        ImageIcon icon = new ImageIcon(imageData);
        flag = true;
        //图标缩放知适合大小，并展示
        icon.setImage(icon.getImage().getScaledInstance(windowLen,windowLen,Image.SCALE_DEFAULT));
        lblIcon.setIcon(icon);
    }

    //更新课堂质量
    public void updateQuarityLabel(double _quarity)
    {
        if(quarity != _quarity)
        {
            Jquarity.setText("课堂质量:  " + _quarity);
            quarity = _quarity;
        }
    }
    //更新教室号标签
    public void updateRoomLabel(String _room)
    {
        if(!classRoom.equals(_room))
        {
            Jroom.setText("教室号:  " + _room);
            classRoom = _room;
        }
    }

    //更新学生数标签
    public void updateStuNumLabel(int _num)
    {
        if(studentNum != _num)
        {
            JstuNum.setText("学生数量:  " + _num);
            studentNum = _num;
        }
    }

    //设置框位置
    public void setIconLocation(int index)
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
        this.setBounds(locationX,locationY,this.windowLen,this.windowLen);
    }
}

