import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    static Socket socket;
    static ObjectOutputStream out;
    static ObjectInputStream in;
    static byte[] byteArr;
    static int numOfBytes;
    static Scanner scn;

    public static void download() throws ClassNotFoundException, IOException{
        String fpath="";
        String fname="";
        String[] token;
        String msg="";
        String outmsg="";

        msg=(String) in.readObject(); //name
        System.out.print(msg);
        outmsg=scn.nextLine();
        //rename option
        System.out.println("Rename: 1.Yes 2.No");
        fname=scn.nextLine();
        if(fname.contains("1")){
            System.out.print("New Name:");
            fname=scn.nextLine();
        }
        else{
            fname=outmsg;
        }
        System.out.println("Download Folder Path:"); //path
        fpath=scn.nextLine();
        out.writeObject(outmsg);                              

        fpath=fpath+"/"+fname;
        msg=(String)in.readObject();
        if(msg.contains("File not found")){
            System.out.println("Server: "+msg);
            return;
        }
        FileOutputStream fo=new FileOutputStream(fpath);
        int cnk=Integer.parseInt(msg);
        byteArr=new byte[cnk];

        while(true){
            msg=(String)in.readObject();
            if(msg.contains("File sent")){
                System.out.println("Download complete");
                break;
            }
            else{
                System.out.println(msg+" received");
                token=msg.split(" ");
                Object o=in.readObject();
                byteArr=(byte[]) o;
                fo.write(byteArr,0,Integer.parseInt(token[1]));
            }
        }
        fo.close();
    
    }
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        String msg="";
        String outmsg="";
        byteArr=null;
        scn=new Scanner(System.in);

        //connection
        socket = new Socket("localhost",6666);
        System.out.println("Server connected...");
        out=new ObjectOutputStream(socket.getOutputStream());
        in=new ObjectInputStream(socket.getInputStream());
       
        msg=(String)in.readObject();
        System.out.print(msg);
        msg=scn.nextLine();
        out.writeObject(msg); //username
        msg = (String) in.readObject();
        System.out.println(msg);

        if(msg.contains("Enter correct username")){
            return;
        }


        while(true){
            msg=(String)in.readObject(); //options
            System.out.println(msg);
            outmsg=scn.nextLine();
            if(outmsg.contains("1")){
                out.writeObject(outmsg);
                msg=(String) in.readObject();
                System.out.println(msg);
            }

            else if(outmsg.contains("2")){
                out.writeObject(outmsg);
                msg=(String) in.readObject(); //type
                System.out.println(msg);
                outmsg=scn.nextLine();
                out.writeObject(outmsg);
                msg=(String) in.readObject(); //files
                System.out.println(msg);

                //downloading
                msg=(String) in.readObject(); 
                System.out.println(msg);
                outmsg=scn.nextLine();
                out.writeObject(outmsg);

                if(outmsg.contains("1")){                     
                    download();
                }      
            }

            else if(outmsg.contains("3")){
                out.writeObject(outmsg);
                msg=(String) in.readObject(); //files
                System.out.println(msg);

                //downloading
                msg=(String) in.readObject(); 
                System.out.println(msg);
                outmsg=scn.nextLine();
                out.writeObject(outmsg);

                if(outmsg.contains("1")){ 
                    msg=(String) in.readObject(); //host
                    System.out.print(msg);
                    outmsg=scn.nextLine();
                    out.writeObject(outmsg);
                    
                    download();
                }               
            }

            else if(outmsg.contains("4")){
                out.writeObject(outmsg);
                msg=(String) in.readObject(); //req id
                System.out.print(msg);
                outmsg=scn.nextLine();
                out.writeObject(outmsg);
                msg=(String) in.readObject(); //description
                System.out.println(msg);
                outmsg=scn.nextLine(); //send description
                out.writeObject(outmsg);
                msg=(String) in.readObject();
                System.out.print(msg);
            }

            else if(outmsg.contains("5")){
                out.writeObject(outmsg);
                msg=(String) in.readObject();
                System.out.print(msg);
            }

            //upload
            else if(outmsg.contains("6")){
                int chunck;
                long fileID;
                String fpath;
                Boolean timeout=false;
                out.writeObject(outmsg);
                msg=(String) in.readObject(); //type
                System.out.println(msg);
                outmsg=scn.nextLine();
                out.writeObject(outmsg);

                if(outmsg.equals("3")){
                    msg=(String)in.readObject();
                    System.out.print(msg);
                    outmsg=scn.nextLine();
                    out.writeObject(outmsg);
                }

                msg=(String) in.readObject(); //name
                System.out.print(msg);
                outmsg=scn.nextLine();
                System.out.println("File Path:");
                fpath=scn.nextLine();
                File file=new File(fpath); //path: D:\L3T2\CSE 322\ASGN1\src\a.txt
                out.writeObject(outmsg);

                msg=(String) in.readObject(); //size
                System.out.print(msg);
                //outmsg=scn.nextLine();
                outmsg=""+file.length();
                System.out.println(outmsg);
                out.writeObject(outmsg);

                //transmission denied... or chunck size
                msg=(String) in.readObject(); 
                System.out.println(msg);

                if(msg.contains("Chunck Size")){
                    String token[]=msg.split(":");
                    msg=token[1];
                    chunck=Integer.parseInt(msg);
                    byteArr=new byte[chunck];

                    msg=(String) in.readObject(); //id
                    System.out.println(msg);
                    token=msg.split(":");
                    msg=token[1];
                    fileID=Long.parseLong(msg);
                    
                    //gfg... send file
                    FileInputStream fi=new FileInputStream(file);
                    while ((numOfBytes=fi.read(byteArr))!=-1){
                        out.writeObject(numOfBytes); 
                        out.writeObject(byteArr); 
                        //timeout
                        socket.setSoTimeout(30000);
                        try{
                            msg=(String) in.readObject();
                            System.out.println(msg);
                        }catch(Exception e){
                            timeout=true;
                            break;
                        }finally{
                            socket.setSoTimeout(0);
                        }
                        byteArr=new byte[chunck];                     
                    }
                    fi.close();
                    if(timeout){
                        in.readObject();
                        out.writeObject("Transmission timeout");
                    }
                    else{
                        out.writeObject("All chunck sent");
                    }
                    msg=(String)in.readObject();
                    System.out.println(msg);
                }
            }

            else if(outmsg.contains("7")){
                out.writeObject(outmsg);
                msg=(String) in.readObject();
                System.out.print(msg);
            }

            else if(outmsg.contains("0")){
                out.writeObject(outmsg);
                break;
            }
            else{
                out.writeObject(outmsg);
            }
        }


    }
}
