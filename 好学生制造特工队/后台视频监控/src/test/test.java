package test;

import java.awt.*;			//导包
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import javax.swing.*;
public class test
{

    public static void writeImageToDisk(byte[] img, String fileName){
        try {
            File file = new File("D:\\" + fileName);
            FileOutputStream fops = new FileOutputStream(file);
            fops.write(img);
            fops.flush();
            fops.close();
            System.out.println("图片已经写入到D盘");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public static void main(String[] args)
    {

        String path = "/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/unquarify/a_137.jpg";
        try {
            FileInputStream fis = new FileInputStream(path);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            byte[] buffer = new byte[65536];
            int count = 0;
            while ((count = fis.read(buffer)) >= 0) {
                baos.write(buffer, 0, count);
            }
            File file = new File(path + "test.jpg");
            FileOutputStream fops = new FileOutputStream(file);
            fops.write(baos.toByteArray());
            fops.flush();
            fops.close();
            baos.close();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }


    }

}

