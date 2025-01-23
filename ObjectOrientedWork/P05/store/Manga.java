public class Manga {
    public static void main(String[] args) {
        String storeName;
        if (args.length > 0) {
            storeName = args[0];
        } else {
            storeName = "Manga Store";
        }
        Store store = new Store(storeName);
        Controller controller = new Controller(store);
        controller.mdi();
    }
}

