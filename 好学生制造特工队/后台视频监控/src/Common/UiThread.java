package Common;

import UI.ImagePanel;
import UI.clientUI;

public class UiThread extends Thread {

    private clientUI ui;

    private String room;

    private frameGetter getter;

    public UiThread(clientUI _ui,String _room,frameGetter _getter)
    {
        ui = _ui;
        room = _room;
        getter = _getter;
    }

    public void run()
    {
        ui.addImagePanel(room,getter);
        /*
        * TODO
        * 这个很重要 在进一步扩展时 要注意多线程同步异步的问题
        * */
        ImagePanel panel = ui.ImapePanelMapPool.get(room);
        panel.getter.startGetFrames(panel);
    }
}
