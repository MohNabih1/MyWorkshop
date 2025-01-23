package store;

public class Customer {

    private String name;
    private String email;

    public Customer(String name, String email) {
        if (!email.contains("@")) {
            throw new IllegalArgumentException("Invalid Email Address: " + email);
        }

        this.name = name;
        this.email = email;
    }

 public String getName() {
        return name;
    }

    @Override
    public String toString() {
        return name + ", Email Address: " + email;
    }
}

