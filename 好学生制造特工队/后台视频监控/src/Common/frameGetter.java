package Common;

import UI.ImagePanel;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayOutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.util.HashMap;
import java.util.Map;

import static Common.util.bytes2Int;

public class frameGetter
{
    private  DatagramSocket sock = null;
    private  DatagramPacket pack = null;

    //frametime + count + order + datalen + data
    private byte[] buf = new byte[8 + 4 + 4 + 4 + util.FRAME_UNIT_MAX];

    public frameGetter(DatagramSocket _sock)
    {
        this.sock = _sock;
        this.pack = new DatagramPacket(buf, buf.length,new InetSocketAddress("localhost",sock.getLocalPort()));
    }

    public void startGetFrames(ImagePanel imagePanel)
    {

        Map<Integer, frameUnit> unitMap = new HashMap<Integer, frameUnit>();

        long preFrameTime = 0;
        System.out.println("strat receive.....");
        while (true) {
            //get a frame
            frameUnit frame = recieve();
            //get new frame id
            long frameTime = frame.getFrameTime();
            if(frameTime < preFrameTime)
            {
                continue;
            }
            else
            {
                if(frameTime > preFrameTime)
                {
                    if(unitMap.size() < frame.getUnitCount()) {
                        imagePanel.tempNum.poll();
                        System.out.println("lose the frame " + frameTime);
                    }

                    preFrameTime = frameTime;
                    unitMap.clear();
                }
                unitMap.put(frame.getUnitOrder(), frame);
                if (unitMap.size() == frame.getUnitCount())
                {
                    byte[] imageData = getImageBytes(unitMap, frame.getUnitCount());
                    System.out.println(sock.getLocalPort()+" flush new image!!!!!!!!!!!!!!!!!!!!!!!!!");
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
                }
            }
        }
    }

    public byte[] getImageBytes(Map<Integer, frameUnit> unitMap, int count) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try {
            for (int i = 0; i < count; i++) {
                frameUnit unit = unitMap.get(i);
                baos.write(unit.getUnitData());
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        return baos.toByteArray();
    }

    //get a frame unit
    public frameUnit recieve() {
        try {
            sock.receive(pack);
        } catch (Exception e) {
            e.printStackTrace();
        }

        frameUnit unit = new frameUnit();

        byte[] frameTime = new byte[8];
        byte[] unitCount = new byte[4];
        byte[] order = new byte[4];
        byte[] dataLen = new byte[4];

        System.arraycopy(buf, 0, frameTime, 0, 8);
        System.arraycopy(buf, 8, unitCount, 0, 4);
        System.arraycopy(buf, 12, order, 0, 4);
        System.arraycopy(buf, 16, dataLen, 0, 4);

        byte[] unitData = new byte[bytes2Int(dataLen)];
        System.arraycopy(buf, 20, unitData, 0, unitData.length);

        unit.setFrameTime(util.bytes2Long(frameTime));
        unit.setUnitCount(util.bytes2Int(unitCount));
        unit.setUnitOrder(util.bytes2Int(order));
        unit.setDataLen(util.bytes2Int(dataLen));
        unit.setUnitData(unitData);

        return unit;
    }

    //更新图标
    public void updateIcon(byte[] dataBytes,JLabel lblIcon,int width,int height) {
        ImageIcon icon = new ImageIcon(dataBytes);
        //图标缩放知适合大小，并展示
        icon.setImage(icon.getImage().getScaledInstance(width,height, Image.SCALE_DEFAULT));
        lblIcon.setIcon(icon);
    }

}
