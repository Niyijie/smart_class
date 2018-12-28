package Common;

public class frameUnit
{
    //时间帧编号
    private long frameTime;

    //切割帧后的总单元数
    private int unitCount;

    //切割帧后的编号
    private int unitOrder;

    //数据长度
    private int dataLen;

    //要传送的数据
    private byte[] unitData;

    public void setFrameTime(long n)
    {
        frameTime = n;
    }

    public void setUnitCount(int n)
    {
        unitCount = n;
    }

    public void setUnitOrder(int n)
    {
        unitOrder = n;
    }

    public void setUnitData(byte[] bytes)
    {
        unitData = bytes;
    }

    public void setDataLen(int len)
    {
        dataLen = len;
    }

    public long getFrameTime() {
        return frameTime;
    }

    public int getUnitCount() {
        return unitCount;
    }

    public int getUnitOrder() {
        return unitOrder;
    }

    public byte[] getUnitData() {
        return unitData;
    }

    public int getDataLen()
    {
        return dataLen;
    }
}
