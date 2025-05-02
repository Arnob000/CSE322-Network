import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class Info {
    public String name;
    public ObjectOutputStream out;
    public ObjectInputStream in;
    public Socket socket;
    public Boolean online;

    public Info(String n, ObjectInputStream i, ObjectOutputStream o, Socket s){
        name=n;
        out=o;
        in=i;
        socket=s;
        online=true;
    }

}
