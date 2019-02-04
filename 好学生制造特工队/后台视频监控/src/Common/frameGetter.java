package Common;

import UI.ImagePanel;
import org.bytedeco.javacpp.opencv_core;
import org.bytedeco.javacv.FFmpegFrameGrabber;
import org.bytedeco.javacv.FrameGrabber;
import org.bytedeco.javacv.Java2DFrameConverter;
import org.bytedeco.javacv.OpenCVFrameConverter;
import org.bytedeco.javacv.Frame;

import org.opencv.core.Mat;
import org.opencv.videoio.VideoCapture;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

import java.net.Socket;

public class frameGetter {
    private Socket sock = null;
    private String rtsp = null;
    private FFmpegFrameGrabber grabber = null;

    public frameGetter(String rtsp,Socket sock) {
        this.sock = sock;
        this.rtsp = rtsp;
    }

    public Socket getClientSock()
    {
        return sock;
    }

    public void startGetFrames(ImagePanel imagePanel) {
        try {
            grabber = FFmpegFrameGrabber.createDefault(this.rtsp);
            grabber.setImageWidth((int) (util.getScreenSize().width*0.85));
            grabber.setImageHeight(util.getScreenSize().height);
            grabber.setOption("rtsp_transport", "tcp");
            grabber.start();
        }
        catch (FrameGrabber.Exception e)
        {
            e.printStackTrace();
            return;
        }
        while (true) {
            try {
                Frame frame = grabber.grab();
                BufferedImage imageData =  new Java2DFrameConverter().getBufferedImage(frame);

                if (!imagePanel.flag)
                    imagePanel.updateIcon(imageData);

                //如果classUI存在 并且打开着
                if (imagePanel.getClassUI()!=null&&imagePanel.getClassUI().getFlag())
                {
                    imagePanel.getClassUI().updateIcon(imageData);
                }

                if(imagePanel.tempNum.size() != 0) {
                    int num = imagePanel.tempNum.poll();
                    imagePanel.updateStuNumLabel(num);
                    if (imagePanel.getClassUI() != null)
                        imagePanel.getClassUI().updateArrivedStudentLabel(num);
                }
                Thread.sleep(10);
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        }
    }

}
