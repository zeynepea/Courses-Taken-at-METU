package ceng.ceng351.labdb;


import java.util.ArrayList;
import java.lang.Math;
import java.util.Iterator;

public class LabDB {

    private int globalDepth;
    private int bucketSize;
    private ArrayList<Integer> bucketAddressTable;
    private ArrayList<Bucket> buckets;

    public LabDB(int bucketSize) {

        this.globalDepth = 1;
        this.bucketSize = bucketSize;
        this.bucketAddressTable = new ArrayList<Integer>(2);
        this.bucketAddressTable.add(0);
        this.bucketAddressTable.add(1);
        this.buckets = new ArrayList<Bucket>(2);
        this.buckets.add(new Bucket(bucketSize,1));
        this.buckets.add(new Bucket(bucketSize,1));
    }

    public void enter(String studentID) {

        int id = Integer.parseInt(studentID.substring(1));
        Double remaining =  id % (Math.pow(2, globalDepth));
        int address = bucketAddressTable.get(remaining.intValue());
        if(buckets.get(address).getNumberOfRecords() < bucketSize){
            buckets.get(address).addRecordsTable(studentID);
        }
        else{
            // increase global depth and address table
            if(buckets.get(address).getLocalDepth() == globalDepth){
                for (int i = 0; i < Math.pow(2, globalDepth); i++){
                    this.bucketAddressTable.add(bucketAddressTable.get(i));
                }
                globalDepth++;
                buckets.get(address).increaseLocalDepthByOne();

                //add new bucket and set it's pointer
                this.buckets.add(new Bucket(bucketSize, buckets.get(address).getLocalDepth()));
                Double x = Math.pow(2, globalDepth-1);
                this.bucketAddressTable.set(remaining.intValue()+x.intValue(), buckets.size() - 1);
            }
            else{
                buckets.get(address).increaseLocalDepthByOne();
                //add new bucket and set it's pointer
                this.buckets.add(new Bucket(bucketSize, buckets.get(address).getLocalDepth()));
                Double x = Math.pow(2, globalDepth);
                int a = buckets.get(address).getLocalDepth();
                Double b = Math.pow(2, a);
                for(int i = remaining.intValue(); i>=0;i=i-b.intValue()){
                    this.bucketAddressTable.set(i, buckets.size() - 1);
                }
                for(int i = remaining.intValue() + b.intValue(); i<x;i=i+b.intValue()){
                    this.bucketAddressTable.set(i, buckets.size() - 1);
                }

            }

            ArrayList<String> records = new ArrayList<>(buckets.get(address).getRecordsTable());
            records.add(studentID);
            buckets.get(address).removeAllRecord();
            for(String record: records){
                int id2 = Integer.parseInt(record.substring(1));
                Double remaining2 =  id2 % (Math.pow(2, globalDepth));
                int address2 = bucketAddressTable.get(remaining2.intValue());
                buckets.get(address2).addRecordsTable(record);
            }
        }
     }

    public void leave(String studentID) {
        if(search(studentID)=="-1"){
            return;
        }
        int id = Integer.parseInt(studentID.substring(1));
        Double remaining =  id % (Math.pow(2, globalDepth));
        int address = bucketAddressTable.get(remaining.intValue());

        buckets.get(address).removeRecord(studentID);
        if(buckets.get(address).getNumberOfRecords() == 0 ){
            Double remaining2;
            if(remaining>=(Math.pow(2, buckets.get(address).getLocalDepth()-1)))
                remaining2 = remaining - (Math.pow(2,  buckets.get(address).getLocalDepth()-1));
            else
                remaining2 = remaining + (Math.pow(2, buckets.get(address).getLocalDepth()-1));
            int address2 = bucketAddressTable.get(remaining2.intValue());

            if(buckets.get(address).getLocalDepth() == buckets.get(address2).getLocalDepth()){
                Double x = Math.pow(2, globalDepth);
                int a = buckets.get(address).getLocalDepth();
                Double b = Math.pow(2, a);
                for(int i = remaining.intValue(); i>=0;i=i-b.intValue()){
                    this.bucketAddressTable.set(i, address2);
                }
                for(int i = remaining.intValue() + b.intValue(); i<x;i=i+b.intValue()){
                    this.bucketAddressTable.set(i, address2);
                }
                if(buckets.get(address).getLocalDepth()!=1)
                    buckets.get(address).decreaseLocalDepthByOne();
                if(address != address2)
                    buckets.get(address2).decreaseLocalDepthByOne();
            }

        }
        int flag = 0;
        for(Bucket bucket: buckets){
            if(bucket.getLocalDepth() == globalDepth){
                if(bucket.getNumberOfRecords()!=0){
                    flag =1;
                    break;
                }
            }
        }
        if(flag == 0){
            globalDepth --;
            Double x = Math.pow(2, globalDepth);
            for(int i = 0 ; i<x ; i++){
                bucketAddressTable.remove(bucketAddressTable.size()-1);
            }
        }
    }

    public String search(String studentID) {
        int id = Integer.parseInt(studentID.substring(1));
        Double remaining =  id % (Math.pow(2, globalDepth));
        int address = bucketAddressTable.get(remaining.intValue());
        ArrayList<String> records = new ArrayList<>(buckets.get(address).getRecordsTable());
        for(String record: records){
            if(record.equals(studentID)){
                String binary = Integer.toString(remaining.intValue(),2);
                if(binary.length()<globalDepth){
                    int y = globalDepth-binary.length();
                    for(int j = 0; j<=y;j++){
                        binary = "0" + binary;
                    }
                }
                return binary;
            }
        }
        return "-1";
    }

    public void printLab() {

        System.out.println("Global depth : " +globalDepth);
        for(int i = 0; i < Math.pow(2, globalDepth); i++){
            String binary = Integer.toString(i,2);
            if(binary.length()<globalDepth){
                int y = globalDepth-binary.length()-1;
                for(int j = 0; j<=y;j++){
                    binary = "0" + binary;
                }
            }
            System.out.print(binary + " : ");
            buckets.get(bucketAddressTable.get(i)).printBucket();
        }
    }
}
