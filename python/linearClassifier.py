import numpy as np 

def parse_file(filename):
    with open(filename, "r") as f:
        data = [[float(y) for y in x.strip().split(",")] for x in f]
    f.close()
    np_data = np.array(data)

    # normalize values
    np_data[:,0] = np_data[:,0]/28
    np_data[:,1] = np_data[:,1]/200
    np_data[:,2] = np_data[:,2]/125
    np_data[:,3] = np_data[:,3]/100
    np_data[:,4] = np_data[:,4]/850
    np_data[:,5] = np_data[:,5]/68
    np_data[:,6] = np_data[:,6]/2.45
    np_data[:,7] = np_data[:,7]/100
    
    return np_data

def get_centroids(training_data):
    p = np.zeros(8)
    n = np.zeros(8)
    num_of_positive = 0
    num_of_negative = 0

    for row in training_data:
        if row[8] == 0:
            num_of_negative += 1
            for index, val in enumerate(row):
                if index < 8:
                    n[index] += val
        if row[8] == 1:
            num_of_positive += 1
            for index, val in enumerate(row):
                if index < 8:
                    p[index] += val

    p = [val/num_of_positive for val in p]
    n = [val/num_of_negative for val in n]
    p = np.array(p)
    n = np.array(n)
    print("Number of training data:",num_of_positive+num_of_negative)
    return p,n  

def get_weight(p,n):
    return p-n

def get_threshold(p,n):
    p_plus_n = (p+n) 
    p_minus_n = (p-n)
    t = p_plus_n.dot(p_minus_n) * 0.5
    return t

def run_classifier(test_data,w,t):
    num_tests = test_data.shape[0]
    num_errors = 0

    for row in test_data:
        classification = row[8]
        row = row[:8]
        if(row.dot(w) > t):
            prediction = 1
        else:
            prediction = 0
        if(prediction != classification):
            num_errors += 1
    print("Number of test data:",num_tests)
    print("Error rate:",num_errors/num_tests)


if __name__ == "__main__":
    """
        Usage: python linearClassifier.py [training file path] [testing file path]
    """
    import sys

    training_input = parse_file(sys.argv[1])
    testing_input = parse_file(sys.argv[2])

    p,n = get_centroids(training_input)
    w = get_weight(p,n)
    t = get_threshold(p,n)
    run_classifier(testing_input,w,t)