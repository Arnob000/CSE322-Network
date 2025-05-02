import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;

public class Server {
    static int curBufferSize=0;
    static Long fileID=(long) 1;
    static HashMap<Long,String> fileList;
        

    public static int getCurBufferSize(){
        return curBufferSize;
    }

    public static void changeCurBufferSize(int size){
        curBufferSize+=size;
    }

    public static void main(String[] args) {
        ServerSocket serverSocket=null;
        HashMap<String,Info> clientList=new HashMap<String, Info>();
        fileList=new HashMap<Long,String>();
        String path="Serverfiles";
        try {
            serverSocket=new ServerSocket(6666);
            System.out.println("Server started...");

            while(true){
                try {
                    Socket socket=serverSocket.accept();
                    System.out.println("Client detected...");
                    new ServerThread(socket,clientList, path);

                } catch (IOException e) {
                    System.out.println("Failed to accept new client");
                }
            }
        } catch (IOException e) {
            System.out.println("Server failed to start");
        }
        
    }
}
