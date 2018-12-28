package test;

import Common.util;
import org.opencv.core.Mat;
import sun.awt.image.BufferedImageDevice;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.awt.*;
import javax.swing.*;

public class test extends JFrame {
//    private static DatagramSocket sock = null;
//
//    static
//    {
//        try
//        {
//            sock = new DatagramSocket();
//        }
//        catch (Exception e)
//        {
//            e.printStackTrace();
//        }
//    }
//
//    public static void main(String[] ags)
//    {
////        try {
////
////            Socket Sock = new Socket("172.20.10.14", 7001);   //客户端tcp套接字
////
////        }
////        catch (Exception e)
////        {
////            e.printStackTrace();
////        }
//
//        List list = new ArrayList<Integer>();
//        list.add(1);
//        list.add(2);
//        System.out.println(list);
//
//    }

    public JLabel videoArea ;
    test() throws Exception {
        setSize(500, 500);
        //设置视频区
        videoArea = new JLabel();
        videoArea.setBounds(0, 0, (int) (0.85 * 500), 500);
        videoArea.setLayout(null);
        this.add(videoArea);
        this.setVisible(true);
        //ImageIcon icon = new ImageIcon("/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/next.jpg");

        BufferedImage buf = ImageIO.read(new File("/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/next.jpg"));
        Graphics g2d = buf.getGraphics();
        g2d.setColor(Color.RED);

        g2d.drawRect(200,100,100,100);
        ImageIcon icon = new ImageIcon(buf);

       // icon.getImage().getGraphics().drawRect(100,100,100,100);
        icon.setImage(icon.getImage().getScaledInstance((int)(0.85*500),500,Image.SCALE_DEFAULT));
        videoArea.setIcon(  icon);
    }

    public static void main(String argsl[]) {
        try {
            test t = new test();

        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}