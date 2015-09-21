#include "gmmnode.h"
#include "data/datafactory.h"
#include "data/messagedata.h"
#include "tabledata/tabledata.h"
#include <QDebug>
#include <QList>
#include <QFile>
#define MAX_SEGMENTS 40;
//------------------------------------------------------------------------------
// Constructor and Destructor

CGmmNode::CGmmNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CGmmNode::configure(CNodeConfig &config)
{
    // Set the node Description
    config.setDescription("The LERAD Anomaly Detection Algorithm: "
                          "Build rules that describe the normal pattern "
                          "of given data.");

    // Add parameters
    //config.addFilename("file", "Input File", "File to be read from disk.");
    config.addUInt("rseed", "Random Seed", "The random seed to feed the "
                   "random number generator at the start.", 666);
    config.addUInt("sample_size", "Training Samples", "Number of samples to use "
                   "for training GMM.", 100);
    config.addUInt("pairs_to_match", "Number of Pairs to Match",
                   "Number of pairs to match for building rules", 1000);
    config.addUInt("max_rules_per_pair", "Maximum Rules per Pair",
                   "Maximum number of rules to create for each pair.", 4);
    config.addBool("dump_rules","Dump Generated Rules","Write the rules "
                   "generated by GMM to a file.", false);
    config.addFilename("rules_file", "Rules Filename", "File where the "
                       "Gaussian Mixture model are written.");

    // Add the gates.
    //config.addInput("in", "table");
    config.addInput("in_train","table");
    config.addInput("in_test","table");
    config.addOutput("out", "table");
}


//------------------------------------------------------------------------------
// Protected Functions

bool CGmmNode::start()
{
    m_ruleset = QSharedPointer<CRulesetData>(
        static_cast<CRulesetData *>(createData("ruleset")));

    if(!m_ruleset.isNull()) {
        return true;
    }
    else {
        return false;
    }
}

bool CGmmNode::data(QString gate_name, const CConstDataPointer &data)
{
    //Q_UNUSED(gate_name);
//------------------------------------------------------------------------------
// Gaussian Mixture model - train and test data
    bool train = false;
    bool test  = false;

    if(gate_name=="in_train"  && data->getType() == "table") {
        // Process table data.
        gmm_train_table = data.staticCast<const CTableData>();
        if(!gmm_train_table.isNull()) {
            trainData(gmm_test_table);
            train = true;
        } else {
            commitError("out", "GMM did not receive a valid training data.");
        }
    }

    if(gate_name=="in_test"  && data->getType() == "table") {
        // Process table data.
        gmm_test_table = data.staticCast<const CTableData>();
        if(!gmm_test_table.isNull() && train==true) {
            testData(gmm_test_table);
            test = true;
        } else {
            commitError("out", "GMM did not receive a valid testing data.");
        }
    }

    if(!gmm_train_table.isNull() && !gmm_test_table.isNull()) {
        if(!train) {
            trainData(gmm_train_table);
            testData(gmm_test_table);
        } else if(train && !test){
            testData(gmm_test_table);
        }
    }

    if(train && test) {
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
//Testing algorithm
void CGmmNode::testData(const QSharedPointer<const CTableData> &table)
{
    qDebug()<<"Test Data";
}

//------------------------------------------------------------------------------
//Training algorithm
void CGmmNode::trainData(const QSharedPointer<const CTableData> &table)
{
    //QString filename = "data.txt";
    //QFile file(filename);
    //file.open(QIODevice::WriteOnly | QIODevice::Text);
    //QTextStream stream( &file );
    qDebug()<<"GMM Data training started...";
    // Number of attributes
    qint32 attribute_count = table->colCount();  // Col 1 --> Time Col 15 --> Length
    qint32 rows = table->rowCount();
    qint32 prev_hour = 0;
    qint32 prev_min  = 0;
    qint32 prev_sec  = 0;
    qlonglong timeDiff = 0.01;//initial offset
    QList<qint32> trainingData = {1,2}; // list of the data to be trained

    for(qint32 j = 0; j < rows; ++j)
    {
        qlonglong rate = 0;
        qint32 length_finder = 9;
        // Get the row.
        const QList<QVariant> &row = table->getRow(j);
         for(qint32 i = 0; i < attribute_count; ++i)
         {
             if(i==1)
             {
                 QString time = row[i].toString();
                 QStringList timeList = time.split(":");
                 qint32 currenthour = timeList[0].toInt();
                 qint32 currentmin  = timeList[1].toInt();
                 qint32 currentsec  = timeList[2].toInt();
                 //qDebug() << "current time obtained";

                 if(j>1) {
                     qint32 hour = currenthour - prev_hour;
                     qint32 min  = currentmin  - prev_min;
                     qint32 sec  = currentsec  - prev_sec;

                     timeDiff = hour*3600 + min*60 + sec;
                     //qDebug() << timeDiff;
                     prev_hour = currenthour;
                     prev_min  = currentmin;
                     prev_sec  = currentsec;
                 }

                 if(j==1) {
                     prev_hour = currenthour;
                     prev_min  = currentmin;
                     prev_sec  = currentsec;
                     //qDebug() << "first loop";
                 }
             } else if(i==attribute_count-length_finder)
             {
                 qint32 length = row[i].toInt();
                 //stream << length <<"\n";
                 trainingData <<length;

                 //rate = length/time
                 if(timeDiff==0)
                 {
                     //use initial offset
                     rate = length/0.01;
                     //qDebug()<< "offset_rate"<<rate;

                 } else
                 {
                     //use current time period
                     rate = length/timeDiff;
                     //qDebug()<< "current_rate"<<rate;
                 }
             }
         }

    } //file.close();
   gmmTrain(trainingData);
}

void CGmmNode::gmmTrain(const QList<qint32> &training_data)
{
    qDebug()<<"Data to be trained... "<< training_data.size();
    float *orig_vector;
    float *in_vector;
    float *labs;
    double datamin;
    double datamax;
    double loglik_old;
    double loglik_new;
    //int max_iterations;
    int i; int j;int k;
    double dsegment_number;
    int empty_levels;
    int greyscale_histogram[256];
    int initial_breakpoints[41];//initial_breakpoints[(MAX_SEGMENTS+1)];
    int current_breakpoint;
    int initialization_error;
    int current_total;
    int done;
    int initialization_multiplier;
    double delta_lookup[256][40];//MAX_SEGMENTS
    double mu[40];//MAX_SEGMENTS
    double sd[40];//MAX_SEGMENTS
    int mu_counter[40];//MAX_SEGMENTS
    double segment_probs[40];//MAX_SEGMENTS
    int sd_counter[40];//MAX_SEGMENTS
    int iter_counter;
    double conv_crit;
    double dnorm_lookup[256][40];
    double denom[256];
    double segment_probs_total;
    double mu_numer;
    double mu_denom;
    double sd_numer;
    double sd_denom;
    double loglik_sum;
    int max_iterations;
    int cardinality[40];//MAX_SEGMENTS
    float ival;
    float pmax;
    int iseg;
    float mlikedenom;
    double test_mu[40];//MAX_SEGMENTS
    float p;

    int segment_number = 6;
    int n = training_data.size();
    /* Allocation of vector storage */
    orig_vector = (float *) malloc ((unsigned) (n+1)*sizeof(float));
    in_vector   = (float *) malloc ((unsigned) (n+1)*sizeof(float));
    labs = (float *) malloc ((unsigned) (n+1)*sizeof(float));

    for (i = 0; i < n; i++)
        in_vector[i] = orig_vector[i];

    datamin  = 1.0E30;
    datamax  = -1.0E30;

    for (i = 0; i < n; i++)
    {
        if (in_vector[i] < datamin) datamin = in_vector[i];
        if (in_vector[i] > datamax) datamax = in_vector[i];
    }

    /* delta_lookup is prob of a datum (we're using histogram) with
        value j being in segment i.
        mu, sd, and segment_probs are the estimated mean, standard deviation,
        and mixture probability for each segment. */

     loglik_old      = 0;              /* Some initializations */
     loglik_new      = 1;
     max_iterations  = 200;
     dsegment_number = segment_number; /* explicit type conversion */

     for (i=0; i<segment_number; i++) {
           mu[i]            = 0.0;
           sd[i]            = 0.0;
           mu_counter[i]    = 0;
           sd_counter[i]    = 0;
           segment_probs[i] = (1.0/dsegment_number);
       }

     /* Rescale values to 0..255 */
     for (i = 0; i < n; i++)
     {
        in_vector[i] = 255.0*(in_vector[i]-datamin)/(datamax-datamin);
      }

     /* Find cardinality of each greyscale level (histogram bin card.) */
      empty_levels=0;

      for (i=0; i<256; i++)
      {
        greyscale_histogram[i] = 0;
        for (j=0; j<n; j++)
        {
            if ((int)in_vector[j]==i) greyscale_histogram[i] += 1;
        }
        if (greyscale_histogram[i]==0) empty_levels+=1;
      }

      /* For the initial segmentation, there are a total of n values to be
            divided (roughly) equally among number_segment initial segments.
            Thus we want about n/number_segment values in each bin.  We use
            initial_breakpoints to store the breakpoints of our binning.
            Note that this will not give a good result when all the points
            are in one histogram bin.  Warning: problems if hist. sparse!
            We should check to make sure the number of unique non-zero grey
            levels (or non-empty histogram bins) is larger than segment_number.
            If not, this should fail gracefully.  */

      initial_breakpoints[0]              = -1;
      initial_breakpoints[segment_number] = 256;
      current_breakpoint                  = -1;
      initialization_error                = 0;

      for (i=1; i<(segment_number); i++)
      {
            current_total = 0;
            done = 0;
            while (done==0) {
                  current_breakpoint += 1;
                  if (current_breakpoint==255) initialization_error+=1;
                  current_total += greyscale_histogram[current_breakpoint];
                  if (current_total >= (n/segment_number) ) {
                     initial_breakpoints[i] = current_breakpoint;
                     done = 1;
                  }
            }
            if (current_total==0) initialization_error+=1;
        }

      initialization_multiplier = 1;
      while (initialization_error > 0) {

            initial_breakpoints[0]              = -1;
            initial_breakpoints[segment_number] = 256;
            current_breakpoint                  = -1;
            initialization_error                = 0;
            initialization_multiplier           = initialization_multiplier*2;

            for (i=1; i<(segment_number); i++) {
                current_total = 0;
                done = 0;
                while (done==0) {
                      current_breakpoint += 1;
                      if (current_breakpoint==255) initialization_error+=1;
                      current_total += greyscale_histogram[current_breakpoint];

                      if (initialization_multiplier == 0) {
                         //printf("Pathological case in histogram. Stopping.\n");
                         //exit(1);
                      }
                      if (initialization_multiplier >= 4096) {
                         //printf("Pathological case in histogram. Stopping.\n");
                         //exit(1);
              }

                      if (current_total >= (int)((float)n/
                      ( (float)initialization_multiplier *
                        (float)segment_number ))) {
                         initial_breakpoints[i] = current_breakpoint;
                         done = 1;
              }
            }
                if (current_total==0) initialization_error+=1;
        }
      }

      for (i=0; i<(segment_number); i++) {
           for (j=0; j<256; j++) {
               if ( (j>initial_breakpoints[i]) &&
                    (j<= initial_breakpoints[(i+1)])) {
                  delta_lookup[j][i] = 1.0; }
               else delta_lookup[j][i] = 0.0;
           }
       }
      /* End of initializing delta_lookup*/

      /* Initialize mu to the starting guess */
        for (i=0; i<segment_number; i++) {
             for (j=0; j<256; j++) {
                 if (delta_lookup[j][i] == 1.0) {
                    mu[i] += (double)j*(double)greyscale_histogram[j];
                    mu_counter[i] += greyscale_histogram[j];
                 }
             }
        }

        for (i=0; i<segment_number; i++) {
              if (mu_counter[i] == 0) {
                //printf("BIC: -99999900.0\n");
                 //erhand("Cluster empty. BIC: NA");
                 qCritical()<<"Cluster empty";
              }
              mu[i] = (mu[i]/(double)mu_counter[i]);
          }

        /* Initialize sd to the starting guess */
        for (i=0; i<segment_number; i++) {
              for (j=0; j<256; j++) {
                  if (delta_lookup[j][i] == 1.0) {
                 sd[i] += (double)greyscale_histogram[j]*
                        pow(((double)j-mu[i]),2);
                     sd_counter[i] += greyscale_histogram[j];
                  }
              }
          }
        for (i=0; i<segment_number; i++) {
              sd[i] = sqrt(sd[i]/((double)sd_counter[i] - 1.0));
              /* Avoid zero std. dev. by giving instead a small value. */
              if (sd[i]==0.0) sd[i] = .25;
          }
        /* Use EM algorithm to find MLE for segment_number segments.
             Note that the first step will have a negative change in
             loglikelihood since the first loglik_old value is zero. */
        iter_counter = 0;

        /* Set the convergence criterion based on the number of observations.
            We want the average change in loglikelihood per val. to be less than
            .00001, so we multiply the number of values by .00001 to get the
            convergence criterion (with an upper bound of 1). */

        conv_crit = (double)n*(0.00001);
        if (conv_crit>1.0) conv_crit=1.0;

        /* Main while loop ************* */
        while ( (fabs(loglik_new-loglik_old) > (float)conv_crit) ||
               (iter_counter==0) ) {

              iter_counter += 1;
              /* E-step */

              /* Only need to calculate the Gaussian probs. for values 0-255
                 for each segment*/
              for (i=0; i<segment_number; i++) {
                  for (j=0; j<256; j++)
                      dnorm_lookup[j][i] = segment_probs[i] *Normi(j,mu[i],sd[i]);
              }

              for (j=0; j<256; j++) denom[j] = 0.0;

              for (i=0; i<segment_number; i++) {
                  for (j=0; j<256; j++) denom[j] = denom[j] + (dnorm_lookup[j][i]);
              }

              /* if denom[j]=0, then there are no points with greyscale level j,
              so delta_lookup will never be called for that value of j unless
              multiplied by greyscale histogram[j] which will be zero. */
              for (i=0; i<segment_number; i++) {
                  for (j=0; j<256; j++) {
                      if (denom[j]>0)
                         delta_lookup[j][i] = dnorm_lookup[j][i]/denom[j];
                      else delta_lookup[j][i] = 0;
                  }
              }

              /* M-step */

              if (fmod(iter_counter,1000)==0)
                 printf("Another 1000 iterations...\n");

              /* update segment_probs */
              for (i=0; i<segment_number; i++) segment_probs[i] = 0.0;

              for (i=0; i<segment_number; i++) {
                  for (j=0; j<256; j++)
                      segment_probs[i]+=(delta_lookup[j][i])*greyscale_histogram[j];
              }

              for (i=0; i<segment_number; i++)
                  segment_probs[i] = segment_probs[i]/(float)n;

              segment_probs_total = 0.0;
              for (i=0; i<segment_number; i++)
                  segment_probs_total += segment_probs[i];

              for (i=0; i<segment_number; i++)
               segment_probs[i] = segment_probs[i]/segment_probs_total;

              /* Update mu */

              for (i=0; i<segment_number; i++) {
                  mu_numer = 0.0;
                  mu_denom = 0.0;

                  for (j=0; j<256; j++) {
                      mu_numer += (delta_lookup[j][i])*(j*greyscale_histogram[j]);
                      mu_denom += (delta_lookup[j][i])*greyscale_histogram[j];
                  }

                  mu[i] = mu_numer/mu_denom;
          }

              /* Update sd */

              for (i=0; i<segment_number; i++) {
                  sd_numer = 0.0;
                  sd_denom = 0.0;

                  for (j=0; j<256; j++) {
                  sd_numer += greyscale_histogram[j]*
                      pow((j-mu[i]),2)*delta_lookup[j][i];
                      sd_denom += greyscale_histogram[j]*delta_lookup[j][i];
                  }

                  sd[i] = sqrt(sd_numer/sd_denom);
                  /* We know that the resolution of our values cannot be
                     better than 1 greyscale level, so sd should not fall
                     below 1/4. */
                  if (sd[i]<.25) { sd[i] = .25; }
          }

              /* Segment probs have been updated, so now update dnorm_lookup*/
              for (i=0; i<segment_number; i++) {
                  for (j=0; j<256; j++)
                      dnorm_lookup[j][i] = segment_probs[i] *Normi(j,mu[i],sd[i]);
              }

              /* Update likelihood */

              loglik_old = loglik_new;
              loglik_new = 0.0;

              for (j=0; j<256; j++) {
                  loglik_sum = 0.0;
                  for (i=0; i<segment_number; i++)
                      loglik_sum += dnorm_lookup[j][i];
                  if (loglik_sum>0) loglik_new +=
                      greyscale_histogram[j]*log(loglik_sum);
                  /* Otherwise, greyscale_histogram will be zero, so add zero. */
          }

        } /* End of while loop.  EM has converged (or diverged beyond precision)*/

        for (k = 0; k < segment_number; k++) cardinality[k] = 0;

         for (i=0; i < n; i++) {
             labs[i] = 0;
             ival = in_vector[i];
             pmax = 0.0;
             iseg = 0;
             mlikedenom = 0.0;
             for (k=0; k<segment_number; k++)
                  mlikedenom += Normr(ival, mu[k], sd[k]) * segment_probs[k];
             for (k=0; k<segment_number; k++) {
                 p = (Normr(ival, mu[k], sd[k]) * segment_probs[k])/mlikedenom;
                 if (p > pmax) {
                pmax = p;
                    iseg = k;
                 }
             }
             /* Increment cluster numbers by one so sequence is 1, 2, ... */
             labs[i] = iseg + 1;
             cardinality[iseg] += 1;
         }

         /* For convenience, redo stdevs from original data */
         for (j=0; j<segment_number; j++) {
             /* Use p for convenience as rectified mean */
             p = ((float)mu[j]*(datamax-datamin)/255.0)+datamin;
             sd[j] = 0.0;
             for (i=0; i< n; i++) {
             if (labs[i] == j+1) {
                    ival = (in_vector[i]*(datamax-datamin)/255.0)+datamin;
                    sd[j] += pow(ival-p,2);
             }
             }
             sd[j] = sqrt(sd[j]/(float)cardinality[j]);
         }

         qDebug()<<"Labels are 1, 2, ... no. clusters.\n";
         qDebug()<<"Segment,  Mu,       SD, Mix.Prob, Card.\n";
         for (i=0; i<segment_number; i++) {
             test_mu[i] = (mu[i]*(datamax-datamin)/255.0)+datamin;
             qDebug()<<(mu[i]*(datamax-datamin)/255.0)+datamin<<"|"
                    <<sd[i]<<"|"
                    <<segment_probs[i]<<"|"
                    <<cardinality[i];
         }

         QString filename = "gmm_train_out.txt";
         QFile file(filename);
         file.open(QIODevice::WriteOnly | QIODevice::Text);
         QTextStream stream( &file );

         for (i=0; i<n; i++)
         {
             stream << labs[i];
         }
         file.close();
}

extern float CGmmNode::Normi(int x, float mu, float sd)
{
    /* Used to compute the Gaussian prob. of integer x. */
             float result,temp;
             temp = (float)x-mu;
             result = (1.0/(sd*sqrt(2*3.141593)))*
                         exp(- pow(temp,2)/(2*pow(sd,2)));
             return(result);
}

extern float CGmmNode::Normr(float x, float mu, float sd)
{
    /* Used to compute the Gaussian probability of real x. */
             float result, temp;
             temp = x-mu;
             result = (1.0/(sd*sqrt(2.0*3.141593)))*
                          exp(- pow(temp,2)/(2*pow(sd,2)));
             return(result);
}