package test;

import Common.util;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Scanner;

public class server {
    private static DatagramSocket sock = null;

    static
    {
        try
        {
            sock = new DatagramSocket(8888);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    static void main(String[] ags)
    {
        Scanner in = new Scanner(System.in);
        byte[] integer = new byte[4];
        DatagramPacket buff = new DatagramPacket(integer, integer.length);
        while(true)
        {
            try
            {
                sock.receive(buff);
                System.out.println(util.bytes2Int(integer));

                DatagramPacket packet = new DatagramPacket(integer, integer.length);
                //47.93.213.146
              //  packet.setSocketAddress();
                sock.send(packet);
                sock.receive(buff);
                System.out.println(util.bytes2Int(integer));
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        }
    }

}