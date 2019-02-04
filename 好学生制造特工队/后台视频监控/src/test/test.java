package test;
import javax.swing.*;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.WritableRaster;
import java.lang.management.MonitorInfo;

import com.sun.javafx.scene.CameraHelper;
import org.bytedeco.javacpp.opencv_core;
import org.bytedeco.javacv.*;
import org.bytedeco.javacv.Frame;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.videoio.VideoCapture;
import org.opencv.videoio.Videoio;

public class test extends JPanel {

    public static void main(String[] args) throws Exception, InterruptedException {
//        OpenCVFrameGrabber grabber = new OpenCVFrameGrabber("/Users/niyijie/Desktop/海葵精英课/web_class/video/1.mp4");
//        grabber.start();   //开始获取摄像头数据
        FFmpegFrameGrabber grabber = FFmpegFrameGrabber.createDefault("rtsp://admin:admin123456@192.168.0.64/Streaming/Channels/1");
        grabber.setOption("rtsp_transport", "tcp");
        //grabber.setFrameRate(25);
       // grabber.setPixelFormat(0); // yuv420p

//
//        JFrame jFrame = new JFrame();//创建jFrame
//        JPanel jPanel = new JPanel();//创建jPanel
//        JLabel lblIcon = new JLabel("test");
//        jPanel.add(lblIcon);//为jPanel添加JLabel
//        jFrame.add(jPanel);//为jFrame添加jPanel
//        jFrame.setVisible(true);//设置可见性
//        jFrame.setSize(500, 500);//设置大小
//
//        lblIcon.setIcon(new ImageIcon("/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/logo.png"));


        grabber.start();

        CanvasFrame canvas = new CanvasFrame("Camera");//新建一个窗口
        canvas.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        canvas.setAlwaysOnTop(true);

        OpenCVFrameConverter.ToIplImage converter = new OpenCVFrameConverter.ToIplImage();

        while (true) {
            Frame frame = grabber.grab();
//            opencv_core.IplImage image = converter.convert(frame);
//
//            BufferedImage bufferedImage = new BufferedImage(image.width(),
//                    image.height(), BufferedImage.TYPE_3BYTE_BGR);
//
//            WritableRaster raster = bufferedImage.getRaster();
//            DataBufferByte dataBuffer = (DataBufferByte) raster.getDataBuffer();
//
//            byte[] imageData = dataBuffer.getData();
//
//            ImageIcon icon = new ImageIcon((new Java2DFrameConverter()).getBufferedImage(frame));
//            icon.setImage(icon.getImage().getScaledInstance(500,500, Image.SCALE_DEFAULT));

          //  lblIcon.setIcon(icon);

//            ImageIcon icon = new ImageIcon(bufferedImage);
//            //图标缩放知适合大小，并展示
//            icon.setImage(icon.getImage().getScaledInstance(700, 700, Image.SCALE_DEFAULT));
//            lblIcon.setIcon(icon);

            if (!canvas.isDisplayable()) {//窗口是否关闭
                grabber.stop();//停止抓取
                System.exit(2);//退出
            }

            canvas.showImage(grabber.grab());//获取摄像头图像并放到窗口上显示， 这里的Frame frame=grabber.grab(); frame是一帧视频图像

            Thread.sleep(10);//10毫秒刷新一次图像
        }

    }
}



//    opencv_core.IplImage image = converter.convert(grabber.grab());
//
//    BufferedImage bufferedImage = new BufferedImage(image.width(),
//            image.height(), BufferedImage.TYPE_3BYTE_BGR);
//    WritableRaster raster = bufferedImage.getRaster();
//    DataBufferByte dataBuffer = (DataBufferByte) raster.getDataBuffer();
//    byte[] imageData = dataBuffer.getData();
//                System.out.println("imageData len " + imageData.length);