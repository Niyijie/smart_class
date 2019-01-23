package Common;

import UI.ImagePanel;
import UI.clientUI;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.DatagramSocket;

public class messageParseFactory {
    public static void parseFactory(clientUI ui, int type) {
        InputStream ips = null;
        //创建套接字信息
        DatagramSocket sock = null;
        frameGetter getter = null;
        //创建套接字流
        OutputStream out = null;
        switch (type) {
            case message.PI_LOGIN_MESSAGE: {
                try {
                    out = ui.clientSock.getOutputStream();
                    ips = ui.clientSock.getInputStream();
                    //获取树莓派端ip信息长度
                    byte[] strLenBytes = new byte[4];
                    ips.read(strLenBytes);
                    System.out.println();
                    //获取客户端地址信息
                    byte[] addrBytes = new byte[util.bytes2Int(strLenBytes)];
                    ips.read(addrBytes);
                    String piAddr = new String(addrBytes);
                    System.out.println(piAddr);

                    //获取输入输出流
                    ByteArrayOutputStream baos = new ByteArrayOutputStream();
                    sock = new DatagramSocket();
                    getter = new frameGetter(sock);
                    //写入udp端口信息 并发送给服务器
                    int UdpPort = sock.getLocalPort();
                    System.out.println("port: " + UdpPort);

                    byte[] portBytes = util.int2Bytes(UdpPort);
                    byte[] msgBytes = util.int2Bytes(message.CLIENT_PORT_MESSAGE);
                    baos.write(msgBytes);
                    baos.write(portBytes);
                    out.write(baos.toByteArray());
                    out.flush();
                    new UiThread(ui, piAddr, getter).start();

                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            }
            case message.PI_BOXS_MESSAGE: {
                try {
                    ips = ui.clientSock.getInputStream();
                    //获取树莓派端ip信息长度
                    byte[] strLenBytes = new byte[4];
                    ips.read(strLenBytes);
                    //获取客户端地址信息
                    byte[] addrBytes = new byte[util.bytes2Int(strLenBytes)];
                    ips.read(addrBytes);
                    String piAddr = new String(addrBytes);
                    //读取框的数量
                    byte[] boxNum = new byte[4];
                    ips.read(boxNum);
                    int nums = util.bytes2Int(boxNum);
                    //更新框的数值
                    ImagePanel imagePanel = ui.ImapePanelMapPool.get(piAddr);
                    imagePanel.tempNum.offer(nums);
//                    imagePanel.updateStuNumLabel(nums);
//                    if(imagePanel.getClassUI() != null)
//                        imagePanel.getClassUI().updateArrivedStudentLabel(nums);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            }
            case message.PI_BOXS_LOCATION_MESSAGE: {
                try {
                    ips = ui.clientSock.getInputStream();

                    //获取树莓派端ip信息长度
                    byte[] strLenBytes = new byte[4];
                    ips.read(strLenBytes);
                    //获取客户端地址信息
                    byte[] addrBytes = new byte[util.bytes2Int(strLenBytes)];
                    ips.read(addrBytes);
                    String piAddr = new String(addrBytes);
                    System.out.println(piAddr);
                    //得到对应地址的panel
                    ImagePanel panel = ui.ImapePanelMapPool.get(piAddr);
                    ;
                    //pi框数量
                    byte[] boxNumBytes = new byte[4];
                    ips.read(boxNumBytes);
                    //获取框的数量，并更新相应数据
                    int boxNum = util.bytes2Int(boxNumBytes);
                    if (panel.getClassUI() != null)
                        panel.getClassUI().updateArrivedStudentLabel(boxNum / 3);
                    panel.updateStuNumLabel(boxNum / 3);
                    //获取坐标，并加入到相应框的坐标数组里
                    panel.getStuBoxsLocation().clear();
                    for (int i = 0; i < boxNum; i++) {
                        byte[] pxy = new byte[4];
                        ips.read(pxy);
                        panel.getStuBoxsLocation().add(util.bytes2Int(pxy));
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            }
            case message.PI_UNQUALIFIED_SCENE_MESSAGE: {
                /*
                 *   TODO
                 *   协议头设计为：
                 *   时间+总人数+专心听课人数+睡觉人数+不专心听课人数+图片
                 */
                try {
                    ips = ui.clientSock.getInputStream();

                    //获取树莓派端ip信息长度
                    byte[] strLenBytes = new byte[4];
                    ips.read(strLenBytes);
                    //获取客户端地址信息
                    byte[] addrBytes = new byte[util.bytes2Int(strLenBytes)];
                    ips.read(addrBytes);
                    String piAddr = new String(addrBytes);
                    //获取时间帧
                    byte[] frameTimeBytes = new byte[8];
                    ips.read(frameTimeBytes);
                    long frameTime = util.bytes2Long(frameTimeBytes);
                    //获取数据长度
                    byte[] datalenBytes = new byte[4];
                    ips.read(datalenBytes);
                    int len = util.bytes2Int(datalenBytes);
                    //获取图像
                    ByteArrayOutputStream baos = new ByteArrayOutputStream();
                    System.out.println("图片字节大小为：。。。。。" + len);
                    while(len > 0)
                    {
                        int recvlen = (len <= 60*1024) ? len:60*1024;
                        byte[] dataBytes = new byte[recvlen];
                        int getLen = ips.read(dataBytes);
                        len = len - getLen;
                        baos.write(dataBytes,0,getLen);
                    }
                    byte[] frameData = baos.toByteArray();
                    System.out.println("获取后大小为: " + frameData.length);
                    //输出文件流
                    String path = "/Users/niyijie/Desktop/智慧课堂/后台视频监控/src/UI/unquarify/"; // 路径
                    util.writeImageToDisk(frameData,(path+frameTime+".jpg"));
                    //写入
                    baos.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            }
        }
    }
}
