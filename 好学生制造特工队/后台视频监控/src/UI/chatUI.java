package UI;

import java.awt.*;			//导包
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;
import javax.swing.*;
public class chatUI extends JFrame {	//继承JFrame顶层框架
    //客户端线程类
    private clientUI client = null;
    //定义组件
    //上部组件
    private JPanel upPart;		//定义面板
    private JSplitPane jsp;	//定义拆分窗格
    private JTextArea historyArea;	//定义文本域
    private JScrollPane historyContainer;	//定义滚动窗格

    private JTextArea inputArea;
    private JScrollPane inputContainer;
    //下部组件
    private JPanel downPart;
    private JButton closeBtn,sendBtn;	//定义按钮
    private JComboBox dropList;		//定义下拉框

    private chatUI self = this;

    private ArrayList<String> selection = null;

    public chatUI(clientUI client)		//构造函数
    {
        //初始化
        this.client = client;
        selection = new ArrayList<>();
        getSelections();
        //创建组件
        //上部组件
        upPart = new JPanel();	//创建面板
        historyArea = new JTextArea();	//创建多行文本框
        historyArea.setLineWrap(true);	//设置多行文本框自动换行
        historyContainer = new JScrollPane(historyArea);	//创建滚动窗格
        inputArea = new JTextArea();
        inputArea.setLineWrap(true);
        inputContainer = new JScrollPane(inputArea);
        jsp = new JSplitPane(JSplitPane.VERTICAL_SPLIT,historyContainer,inputContainer); //创建拆分窗格
        jsp.setDividerLocation(200);	//设置拆分窗格分频器初始位置
        jsp.setDividerSize(1);			//设置分频器大小
        //下部组件
        downPart = new JPanel();
        closeBtn =new JButton("关闭");		//创建按钮
        closeBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                self.setVisible(false);
            }
        });

        sendBtn =new JButton("发送");
        sendBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String send = "发送给" + dropList.getSelectedItem()+":\n";
                if ("".equals(inputArea.getText()))
                    return;
                historyArea.append(send + "\t" +inputArea.getText()+"\n");
                inputArea.setText("");
            }
        });

        String [] name= {"A304","B607","102","C504","T101","所有人"};
        if(selection.size() == 0)
            dropList = new JComboBox(name);	//创建下拉框
        else
            dropList = new JComboBox(selection.toArray());

        //设置布局管理
        upPart.setLayout(new BorderLayout());	//设置面板布局
        downPart.setLayout(new FlowLayout(FlowLayout.RIGHT));

        //添加组件
        upPart.add(jsp);
        downPart.add(dropList);
        downPart.add(closeBtn);
        downPart.add(sendBtn);

        this.add(upPart,BorderLayout.CENTER);
        this.add(downPart,BorderLayout.SOUTH);

        //设置窗体实行
        this.setTitle("消息界面");		//设置界面标题
        this.setSize(400, 350);				//设置界面像素
        this.setLocation(200, 200);			//设置界面初始位置
     //   this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);	//设置虚拟机和界面一同关闭
        this.setVisible(true);				//设置界面可视化
    }


    private void getSelections()
    {
        Iterator<String> it = client.ImapePanelMapPool.keySet().iterator();
        while(it.hasNext())
        {
            selection.add(it.next());
        }
    }
}

